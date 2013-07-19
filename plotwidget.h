#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_picker.h>
#include <qwt_legend.h>
#include <QSettings>
#include <QList>

class LegendItem;
class QwtPlotGrid;
class Curve;

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QwtPlot
{
    Q_OBJECT

public:
    explicit PlotWidget(QWidget *parent = 0);
    virtual ~PlotWidget();


    void setName(const QString &name) {
        mName = name;
    }
    QString getName() const {
        return mName;
    }
private:
    void initZoom();
    void initGrid();
    void setupPlot();

    void insertCurve();

    void setPrecision();



public Q_SLOTS:
    void applySettings(const QString &plotName );
    void configurationChanged();

public:
    virtual void replot();
    void setPrecision(int precision);

private:
    Ui::PlotWidget *ui;
    QwtPlotZoomer *mPlotZoomer;

    QwtLegend *mExternalLegend;
    LegendItem *mLegendItem;
    bool mIsDirty;

    QwtPlotGrid *mPlotGrid;

    QString mName;
};

#endif // PLOTWIDGET_H
