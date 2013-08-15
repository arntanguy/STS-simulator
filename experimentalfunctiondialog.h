#ifndef EXPERIMENTALFUNCTIONDIALOG_H
#define EXPERIMENTALFUNCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ExperimentalFunctionDialog;
}

class ExperimentalFunction;

class ExperimentalFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExperimentalFunctionDialog(QWidget *parent = 0);
    ~ExperimentalFunctionDialog();

    void loadFromFunction(ExperimentalFunction *f);

private:
    void init();

public Q_SLOTS:
    void accept();
    void loadDataFile();
    void dataFileChanged(int);
    void functionChanged(int);

private:
    Ui::ExperimentalFunctionDialog *ui;

    ExperimentalFunction *mSelectedFunction;
};

#endif // EXPERIMENTALFUNCTIONDIALOG_H
