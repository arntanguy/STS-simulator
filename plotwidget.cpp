#include "plotwidget.h"
#include "ui_plotwidget.h"
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include "curve.h"
#include "settings.h"
#include "legenditem.h"

/*!
 * \brief PlotWidget::PlotWidget
 * \param parent
 */
PlotWidget::PlotWidget(QWidget *parent) :
    QwtPlot(parent),
    ui(new Ui::PlotWidget),
     mExternalLegend( NULL ),
     mLegendItem( NULL ),
     mIsDirty( false )
{
    ui->setupUi(this);


//    QwtPlotCurve *curve1 = new QwtPlotCurve("Curve 1");
//
//    QVector< double > xdata;
//    QVector< double > ydata;
//
//
//    for(int i=0;i<100; i++) {
//        xdata.append(i);
//        ydata.append(sin(i));
//        xdata.append(i+0.5);
//        ydata.append(sin(i+0.5));
//    }
//
//
//    curve1->setSamples(xdata, ydata);
//    curve1->setCurveAttribute(QwtPlotCurve::Fitted, true);
//    curve1->attach(this);
//
    setupPlot();
    initZoom();

    replot();
}

PlotWidget::~PlotWidget()
{
    delete ui;
}

/*!
 * \brief PlotWidget::initZoom Initialize the zoom functionalities
 */
void PlotWidget::initZoom()
{
    // Create plot zoomer linked to current plot
    // Attach point is on the canvas, not the plot!
    mPlotZoomer = new QwtPlotZoomer( this->canvas() );
    mPlotZoomer->setKeyPattern( QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier );
    mPlotZoomer->setKeyPattern( QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier );
    mPlotZoomer->setKeyPattern( QwtEventPattern::KeyHome, Qt::Key_Home );

    const QColor c(Qt::darkBlue);
    mPlotZoomer->setRubberBandPen(c);
    mPlotZoomer->setTrackerPen(c);
}

void PlotWidget::setupPlot() {
    this->setTitle("Qwt Tutorial");
    this->setCanvasBackground(QColor(Qt::white));

    // axis legends
    this->setAxisTitle(QwtPlot::xBottom, "V");
    this->setAxisTitle(QwtPlot::yLeft, "P<sub>T</sub>(V)");

}


void PlotWidget::insertCurve()
{
    static int counter = 1;

    const char *colors[] =
    {
        "LightSalmon",
        "SteelBlue",
        "Yellow",
        "Fuchsia",
        "PaleGreen",
        "PaleTurquoise",
        "Cornsilk",
        "HotPink",
        "Peru",
        "Maroon"
    };
    const int numColors = sizeof( colors ) / sizeof( colors[0] );

    QwtPlotCurve *curve = new Curve( counter++ );
    curve->setPen( QColor( colors[ counter % numColors ] ), 2 );
    curve->attach( this );
}

void PlotWidget::applySettings( const Settings &settings )
{
    mIsDirty = false;
    setAutoReplot( true );

    if ( settings.legend.isEnabled )
    {
        if ( settings.legend.position > QwtPlot::TopLegend )
        {
            if ( legend() )
            {
                // remove legend controlled by the plot
                insertLegend( NULL );
            }

            if ( mExternalLegend == NULL )
            {
                mExternalLegend = new QwtLegend();
                mExternalLegend->setWindowTitle("Plot Legend");

                connect(
                    this,
                    SIGNAL( legendDataChanged( const QVariant &,
                        const QList<QwtLegendData> & ) ),
                    mExternalLegend,
                    SLOT( updateLegend( const QVariant &,
                        const QList<QwtLegendData> & ) ) );

                mExternalLegend->show();

                // populate the new legend
                updateLegend();
            }
        }
        else
        {
            delete mExternalLegend;
            mExternalLegend = NULL;

            if ( legend() == NULL ||
                plotLayout()->legendPosition() != settings.legend.position )
            {
                insertLegend( new QwtLegend(),
                    QwtPlot::LegendPosition( settings.legend.position ) );
            }
        }
    }
    else
    {
        insertLegend( NULL );

        delete mExternalLegend;
        mExternalLegend = NULL;
    }

    if ( settings.legendItem.isEnabled )
    {
        if ( mLegendItem == NULL )
        {
            mLegendItem = new LegendItem();
            mLegendItem->attach( this );
        }

        mLegendItem->setMaxColumns( settings.legendItem.numColumns );
        mLegendItem->setAlignment( Qt::Alignment( settings.legendItem.alignment ) );
        mLegendItem->setBackgroundMode(
            QwtPlotLegendItem::BackgroundMode( settings.legendItem.backgroundMode ) );
        if ( settings.legendItem.backgroundMode ==
            QwtPlotLegendItem::ItemBackground )
        {
            mLegendItem->setBorderRadius( 4 );
            mLegendItem->setMargin( 0 );
            mLegendItem->setSpacing( 4 );
            mLegendItem->setItemMargin( 2 );
        }
        else
        {
            mLegendItem->setBorderRadius( 8 );
            mLegendItem->setMargin( 4 );
            mLegendItem->setSpacing( 2 );
            mLegendItem->setItemMargin( 0 );
        }

        QFont font = mLegendItem->font();
        font.setPointSize( settings.legendItem.size );
        mLegendItem->setFont( font );
    }
    else
    {
        delete mLegendItem;
        mLegendItem = NULL;
    }

    QwtPlotItemList curveList = itemList( QwtPlotItem::Rtti_PlotCurve );
    if ( curveList.size() != settings.curve.numCurves )
    {
        while ( curveList.size() > settings.curve.numCurves )
        {
            QwtPlotItem* curve = curveList.takeFirst();
            delete curve;
        }

        for ( int i = curveList.size(); i < settings.curve.numCurves; i++ )
            insertCurve();
    }

    curveList = itemList( QwtPlotItem::Rtti_PlotCurve );
    for ( int i = 0; i < curveList.count(); i++ )
    {
        Curve* curve = static_cast<Curve*>( curveList[i] );
        curve->setCurveTitle( settings.curve.title );

        int sz = 0.5 * settings.legendItem.size;
        curve->setLegendIconSize( QSize( sz, sz ) );
    }

    setAutoReplot( false );
    if ( mIsDirty )
    {
        mIsDirty = false;
        replot();
    }
}

void PlotWidget::replot()
{
    if ( autoReplot() )
    {
        mIsDirty = true;
        return;
    }

    QwtPlot::replot();
}
