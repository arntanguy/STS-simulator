#ifndef DIFFERENTIALCURVEDIALOG_H
#define DIFFERENTIALCURVEDIALOG_H

#include <QDialog>

namespace Ui {
class DifferentialFunctionDialog;
}

class DifferentialFunction;

class DifferentialFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DifferentialFunctionDialog(QWidget *parent = 0);
    explicit DifferentialFunctionDialog(DifferentialFunction *f, QWidget *parent = 0);
    ~DifferentialFunctionDialog();

    void setFunction(DifferentialFunction* f);

public Q_SLOTS:
    void accept();
    void selectFunction();

private:
    void init();
    void functionSelected();

private:
    Ui::DifferentialFunctionDialog *ui;


    DifferentialFunction *mFunction;

    bool mEdit;
};

#endif // DIFFERENTIALCURVEDIALOG_H
