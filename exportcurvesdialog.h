#ifndef EXPORTCURVESDIALOG_H
#define EXPORTCURVESDIALOG_H

#include <QDialog>

namespace Ui {
class ExportCurvesDialog;
}

class QStandardItem;

class ExportCurvesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportCurvesDialog(QWidget *parent = 0);
    ~ExportCurvesDialog();

private:
    void init();
    void initCurves();
    bool exportData(const QString&);

public Q_SLOTS:
    void chooseFile();
    void accept();

private:
    Ui::ExportCurvesDialog *ui;

    QList<QStandardItem *> mItems;
};

#endif // EXPORTCURVESDIALOG_H
