#ifndef PLOTCONTROLDIALOG_H
#define PLOTCONTROLDIALOG_H

#include <QDialog>

namespace Ui {
class PlotControlDialog;
}

class PlotControlDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlotControlDialog(const QString &plotName, QWidget *parent = 0);
    ~PlotControlDialog();
    
private:
    Ui::PlotControlDialog *ui;
    QString mPlotName;
};

#endif // PLOTCONTROLDIALOG_H
