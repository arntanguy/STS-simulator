#include "plotwidget.h"
#include "ui_plotwidget.h"
#include <qwt_plot_curve.h>

/*!
 * \brief PlotWidget::PlotWidget
 * \param parent
 */
PlotWidget::PlotWidget(QWidget *parent) :
    QwtPlot(parent),
    ui(new Ui::PlotWidget)
{
    ui->setupUi(this);


    QwtPlotCurve *curve1 = new QwtPlotCurve("Curve 1");

    QVector< double > xdata;
    QVector< double > ydata;


    for(int i=0;i<100; i++) {
        xdata.append(i);
        ydata.append(sin(i));
        xdata.append(i+0.5);
        ydata.append(sin(i+0.5));
    }


    curve1->setSamples(xdata, ydata);
    curve1->setCurveAttribute(QwtPlotCurve::Fitted, true);
    curve1->attach(this);

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
