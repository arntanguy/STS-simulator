#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

#include <qwt_plot.h>

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QwtPlot
{
    Q_OBJECT
    
public:
    explicit PlotWidget(QWidget *parent = 0);
    ~PlotWidget();
    
private:
    Ui::PlotWidget *ui;
};

#endif // PLOTWIDGET_H
