#ifndef PLOTCONTROLWINDOW_H
#define PLOTCONTROLWINDOW_H

#include <QMainWindow>
#include <QSettings>

class QStandardItem;
class PlotArea;
class PlotWidget;
class QModelIndex;
class FunctionCurve;

namespace Ui {
class PlotControlWindow;
}

class PlotControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotControlWindow(unsigned int plotId, PlotArea *parent = 0);
    ~PlotControlWindow();

private:
    Ui::PlotControlWindow *ui;
    unsigned int mPlotId;
    PlotWidget *mPlot;

    QList<QStandardItem *> mCurveItems;
    QList<QStandardItem *> mFunctionItems;

private:
    void init();
    void initFromConfig();
    FunctionCurve* manageFunctionCurveFromItem(QStandardItem *item);

Q_SIGNALS:
    void accepted();

public slots:
    void newCurve();
    void editCurve(const QModelIndex &);
    void newCurveAvailable();

    void newFunction();
    void newHierarachicalFunction();
    void newIntegralFunction();
    void newDifferentialFunction();
    void newFunctionAvailable();
    void editFunction(const QModelIndex &);
    void editFunctionCurve();
    void deleteFunction();

    void accept();

    void functionDialogAccepted();
};

#endif // PLOTCONTROLDIALOG_H
