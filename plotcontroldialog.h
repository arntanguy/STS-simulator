#ifndef PLOTCONTROLDIALOG_H
#define PLOTCONTROLDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class PlotControlDialog;
}

class PlotControlDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlotControlDialog(const QString &plotName, QWidget *parent = 0);
    ~PlotControlDialog();

    virtual void accept();
    
private:
    Ui::PlotControlDialog *ui;
    QString mPlotName;
    QSettings mSettings;

private:
    void init();
    void initFromConfig();

public slots:
    void autoAbscissaChecked(bool checked);
    void autoOrdinateChecked(bool checked);
};

#endif // PLOTCONTROLDIALOG_H
