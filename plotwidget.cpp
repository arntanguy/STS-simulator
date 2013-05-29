#include "plotwidget.h"
#include "ui_plotwidget.h"
#include <qwt_plot_curve.h>

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

    replot();
}

PlotWidget::~PlotWidget()
{
    delete ui;
}
