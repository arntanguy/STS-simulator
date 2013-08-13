#ifndef EXPORTCURVESDIALOG_H
#define EXPORTCURVESDIALOG_H

#include <QDialog>

namespace Ui {
class ExportCurvesDialog;
}

class ExportCurvesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportCurvesDialog(QWidget *parent = 0);
    ~ExportCurvesDialog();

private:
    void init();
    void initCurves();

public Q_SLOTS:
    void chooseFile();
    void accept();

private:
    Ui::ExportCurvesDialog *ui;
};

#endif // EXPORTCURVESDIALOG_H