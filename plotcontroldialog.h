#ifndef PLOTCONTROLDIALOG_H
#define PLOTCONTROLDIALOG_H

#include <QDialog>
#include <QSettings>

class QStandardItem;
class PlotArea;
class PlotWidget;
class QModelIndex;

namespace Ui {
class PlotControlDialog;
}

class PlotControlDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlotControlDialog(const QString &plotName, PlotArea *parent = 0);
    ~PlotControlDialog();

    virtual void accept();
    
private:
    Ui::PlotControlDialog *ui;
    QString mPlotName;
    PlotWidget *mPlot;

    QList<QStandardItem *> mCurveItems;
    QList<QStandardItem *> mFunctionItems;

private:
    void init();
    void initFromConfig();

public slots:
    void autoAbscissaChecked(bool checked);
    void autoOrdinateChecked(bool checked);
    void newCurve();
    void editCurve(const QModelIndex &);
    void newCurveAvailable();

    void newFunction();
    void newHierarachicalFunction();
    void newFunctionAvailable();
    void editFunction(const QModelIndex &);
};

#endif // PLOTCONTROLDIALOG_H
