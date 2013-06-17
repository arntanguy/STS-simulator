#ifndef PLOTCONTROLDIALOG_H
#define PLOTCONTROLDIALOG_H

#include <QDialog>
#include <QSettings>

class QStandardItem;
class PlotArea;
class PlotWidget;

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

private:
    void init();
    void initFromConfig();

public slots:
    void autoAbscissaChecked(bool checked);
    void autoOrdinateChecked(bool checked);
    void newCurve();
    void newCurveAvailable();
};

#endif // PLOTCONTROLDIALOG_H
