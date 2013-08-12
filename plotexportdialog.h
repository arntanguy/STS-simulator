#ifndef PLOTEXPORTDIALOG_H
#define PLOTEXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class PlotExportDialog;
}

class QwtPlot;

class PlotExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlotExportDialog(QWidget *parent = 0);
    ~PlotExportDialog();

    void addPlot(QwtPlot *p);


public Q_SLOTS:
    void chooseFile();
    void accept();

private:
    Ui::PlotExportDialog *ui;
    QList<QwtPlot *> mPlots;
};

#endif // PLOTEXPORTDIALOG_H
