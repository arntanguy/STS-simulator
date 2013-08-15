#ifndef EXPERIMENTALFUNCTIONDIALOG_H
#define EXPERIMENTALFUNCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ExperimentalFunctionDialog;
}

class ExperimentalFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExperimentalFunctionDialog(QWidget *parent = 0);
    ~ExperimentalFunctionDialog();

private:
    void init();

public Q_SLOTS:
    void accept();
    void loadDataFile();
    void dataFileChanged(int);

private:
    Ui::ExperimentalFunctionDialog *ui;

    bool mEdit;
};

#endif // EXPERIMENTALFUNCTIONDIALOG_H
