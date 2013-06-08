#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_picker.h>
#include <qwt_legend.h>

class LegendItem;
class Settings;

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QwtPlot
{
    Q_OBJECT
    
public:
    explicit PlotWidget(QWidget *parent = 0);
    virtual ~PlotWidget();
    
private:
    Ui::PlotWidget *ui;
    QwtPlotZoomer *mPlotZoomer;

    QwtLegend *mExternalLegend;
    LegendItem *mLegendItem;
    bool mIsDirty;

private:
    void initZoom();
    void setupPlot();

    void insertCurve();



public Q_SLOTS:
    void applySettings( const Settings & );

public:
    virtual void replot();
};

#endif // PLOTWIDGET_H
