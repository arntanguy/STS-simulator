#include "plotwidget.h"
#include "ui_plotwidget.h"
#include <QSettings>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include "curve.h"
#include "settings.h"
#include "legenditem.h"

#include <QPushButton>
#include <qwt_widget_overlay.h>

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

    setupPlot();
    initZoom();
    initOverlay();

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

void PlotWidget::initOverlay()
{
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

/**
 * @brief PlotWidget::applySettings
 *      Apply graph settings from the saved settings (using QSettings)
 * @param plotName
 *      Name of the plot
 */
void PlotWidget::applySettings( const QString &plotName)
{
    QSettings settings;
    settings.beginGroup("Plot/"+plotName+"/legend");

    mIsDirty = false;
    setAutoReplot( true );

    if ( settings.value("isEnabled", false).toBool() )
    {
        QwtPlot::LegendPosition legendPosition = static_cast<QwtPlot::LegendPosition>(settings.value("position" , QwtPlot::TopLegend).toInt());
        if ( legendPosition > QwtPlot::TopLegend )
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
                plotLayout()->legendPosition() != legendPosition)
            {
                insertLegend( new QwtLegend(),
                    QwtPlot::LegendPosition( legendPosition ) );
            }
        }
    }
    else
    {
        insertLegend( NULL );

        delete mExternalLegend;
        mExternalLegend = NULL;
    }
    settings.endGroup();

    settings.beginGroup("Plot/"+plotName+"/legendItem");
    if ( settings.value("isEnabled", true).toBool() )
    {
        if ( mLegendItem == NULL )
        {
            mLegendItem = new LegendItem();
            mLegendItem->attach( this );
        }

        mLegendItem->setMaxColumns( settings.value("numColumns", 1).toInt() );
        QVariant v = static_cast<int>(Qt::AlignRight | Qt::AlignTop);
        mLegendItem->setAlignment( Qt::Alignment( settings.value("alignment", v ).value<int>() ) );
        QwtPlotLegendItem::BackgroundMode bgMode = static_cast<QwtPlotLegendItem::BackgroundMode>(settings.value("backgroundMode", 0).toInt());
        mLegendItem->setBackgroundMode(
            QwtPlotLegendItem::BackgroundMode( bgMode ) );
        if ( bgMode ==
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
        font.setPointSize( settings.value("size", 10).toInt());
        mLegendItem->setFont( font );
    }
    else
    {
        delete mLegendItem;
        mLegendItem = NULL;
    }

    QwtPlotItemList curveList = itemList( QwtPlotItem::Rtti_PlotCurve );
    int numCurves = settings.value("numCurves", 4).toInt();
    if ( curveList.size() != numCurves)
    {
        while ( curveList.size() > numCurves)
        {
            QwtPlotItem* curve = curveList.takeFirst();
            delete curve;
        }

        for ( int i = curveList.size(); i < numCurves; i++ )
            insertCurve();
    }

    curveList = itemList( QwtPlotItem::Rtti_PlotCurve );
    for ( int i = 0; i < curveList.count(); i++ )
    {
        Curve* curve = static_cast<Curve*>( curveList[i] );
        curve->setCurveTitle( settings.value("title", "Curve").toString() );

        int sz = 0.5 * settings.value("size", 10).toInt();
        curve->setLegendIconSize( QSize( sz, sz ) );
    }

    setAutoReplot( false );
    if ( mIsDirty )
    {
        mIsDirty = false;
        replot();
    }
    settings.endGroup();
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
