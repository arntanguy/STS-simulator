#include "plotwidget.h"
#include "ui_plotwidget.h"
#include <QSettings>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include "randomcurve.h"
#include "curve.h"
#include "legenditem.h"
#include "projectsingleton.h"

#include <QPushButton>
#include <qwt_widget_overlay.h>
#include <qwt_date_scale_engine.h>
#include <qwt_plot_grid.h>

/*!
 * \brief PlotWidget::PlotWidget
 * \param parent
 */
PlotWidget::PlotWidget(QWidget *parent) :
    QwtPlot(parent),
    ui(new Ui::PlotWidget),
     mExternalLegend( NULL ),
     mLegendItem( NULL ),
     mIsDirty( false ),
     mName( "Plot" )
{
    ui->setupUi(this);

    mSettings = Singleton<ProjectSingleton>::Instance().getSettings();

    setupPlot();
    initZoom();
    initGrid();

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

void PlotWidget::initGrid()
{
    mPlotGrid = new QwtPlotGrid();
    mPlotGrid->attach(this);
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

    //QwtPlotCurve *curve = new RandomCurve( counter++ );
    //curve->setPen( QColor( colors[ counter % numColors ] ), 2 );
    //curve->attach( this );
    Curve *curve = new Curve( QString("Curve ")+counter );
    curve->setPen( QColor( colors[ counter % numColors ] ), 2 );
    curve->attach( this );
    counter++;
}

/**
 * @brief PlotWidget::applySettings
 *      Apply graph mSettings->from the saved mSettings->(using QSettings)
 * @param plotName
 *      Name of the plot
 */
void PlotWidget::applySettings( const QString &plotName)
{
    mSettings->beginGroup("Plot/"+plotName+"/legend");

    mIsDirty = false;
    setAutoReplot( true );

    if ( mSettings->value("isEnabled", false).toBool() )
    {
        QwtPlot::LegendPosition legendPosition = static_cast<QwtPlot::LegendPosition>(mSettings->value("position" , QwtPlot::BottomLegend).toInt());
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
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+plotName+"/legendItem");
    if ( mSettings->value("isEnabled", true).toBool() )
    {
        if ( mLegendItem == NULL )
        {
            mLegendItem = new LegendItem();
            mLegendItem->attach( this );
        }

        mLegendItem->setMaxColumns( mSettings->value("numColumns", 1).toInt() );
        QVariant v = static_cast<int>(Qt::AlignRight | Qt::AlignTop);
        mLegendItem->setAlignment( Qt::Alignment( mSettings->value("alignment", v ).value<int>() ) );
        QwtPlotLegendItem::BackgroundMode bgMode = static_cast<QwtPlotLegendItem::BackgroundMode>(mSettings->value("backgroundMode", 0).toInt());
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
        font.setPointSize( mSettings->value("size", 10).toInt());
        mLegendItem->setFont( font );
    }
    else
    {
        delete mLegendItem;
        mLegendItem = NULL;
    }

    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+plotName+"/info");
    setTitle(mSettings->value("title", plotName).toString());
    // axis legends
    setAxisTitle(QwtPlot::xBottom, mSettings->value("horizontalAxisName", "X Axis").toString());
    setAxisTitle(QwtPlot::yLeft, mSettings->value("verticalAxisName", "X Axis").toString());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+plotName+"/axisScale");
    QString scale = mSettings->value("horizontalAxisScale", "linear").toString();
    if(scale == "log10") {
        setAxisScaleEngine(QwtPlot::xBottom, new QwtLogScaleEngine());
    } else {
        setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine());
    }
    scale = mSettings->value("verticalAxisScale", "linear").toString();
    if(scale == "log10") {
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine());
    } else {
        setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
    }
    mSettings->endGroup();

    QwtPlotItemList curveList = itemList( QwtPlotItem::Rtti_PlotCurve );
    int numCurves = mSettings->value("numCurves", 4).toInt();
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
        RandomCurve* curve = static_cast<RandomCurve*>( curveList[i] );
        curve->setCurveTitle( mSettings->value("title", "Curve").toString() );

        int sz = 0.5 * mSettings->value("size", 10).toInt();
        curve->setLegendIconSize( QSize( sz, sz ) );
    }

    // Curves must be added before that call!
    mSettings->beginGroup("Plot/"+plotName+"/precision");
    setPrecision(mSettings->value("resolution", 1000).toDouble());
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+plotName+"/range");
    if(!mSettings->value("autoAbscissa", true).toBool()) {
        double min = mSettings->value("minAbscissa", -10.d).toDouble();
        double max = mSettings->value("maxAbscissa", 10.d).toDouble();
        this->setAxisScale(QwtPlot::xBottom, min, max);
    } else {
        this->setAxisAutoScale(QwtPlot::xBottom, true);
    }
    if(!mSettings->value("autoOrdinate", true).toBool()) {
        double min = mSettings->value("minOrdinate", -10.d).toDouble();
        double max = mSettings->value("maxOrdinate", 10.d).toDouble();
        this->setAxisScale(QwtPlot::yLeft, min, max);
    } else {
        this->setAxisAutoScale(QwtPlot::yLeft, true);
    }
    // Replot and then set zoom base to the current axis scale.
    QwtPlot::replot();
    mPlotZoomer->setZoomBase();
    mSettings->endGroup();

    mSettings->beginGroup("Plot/"+plotName+"/grid");
    if(mSettings->value("isEnabled", true).toBool())
        mPlotGrid->attach(this);
    else
        mPlotGrid->detach();

    mPlotGrid->enableX(mSettings->value("majorPen/abscissiaIsEnabled", true).toBool());
    mPlotGrid->enableY(mSettings->value("majorPen/ordinateIsEnabled", true).toBool());
    mPlotGrid->enableXMin(mSettings->value("minorPen/abscissiaIsEnabled", false).toBool());
    mPlotGrid->enableYMin(mSettings->value("minorPen/ordinateIsEnabled", false).toBool());
    Qt::PenStyle penStyle = static_cast<Qt::PenStyle>(mSettings->value("majorPen/style", Qt::SolidLine).toInt());
    mPlotGrid->setMajorPen(Qt::black, mSettings->value("majorPen/width", 0.0).toDouble(), penStyle);
    penStyle = static_cast<Qt::PenStyle>(mSettings->value("minorPen/style", Qt::DotLine).toInt());
    mPlotGrid->setMinorPen(Qt::black, mSettings->value("minorPen/width", 0.0).toDouble(), penStyle);
    mSettings->endGroup();

    setAutoReplot( false );
    if ( mIsDirty )
    {
        mIsDirty = false;
        replot();
    }
}

void PlotWidget::replot()
{
 /*   if ( autoReplot() )
    {
        mIsDirty = true;
        return;
    }
*/

    QwtPlot::replot();
}

/*!
 * \brief PlotWidget::setPrecision
 *  Assign the precision to the curves.
 *  It the new precision is different from the current function,
 *  calling this function will cause all the curves to be update (new values computed),
 *  and the full plot will have to be redrawn.
 *  This operation might be quite expensive, use with caution.
 * \param precision
 *  The new precision to apply (number of points to be computed).
 */
void PlotWidget::setPrecision(int precision)
{
    QwtPlotItemList items = QwtPlotDict::itemList();
    for(auto it = items.begin(); it != items.end(); it++) {
        Curve *curve = 0;
        curve = dynamic_cast<Curve *>(*it);
        if(curve != 0) {
            qDebug() << "Curve " << curve->title().text() ;
            curve->setResolution(precision);
        }
    }
}
