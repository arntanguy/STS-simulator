#ifndef DIFFERENTIALCURVEDIALOG_H
#define DIFFERENTIALCURVEDIALOG_H

#include <QDialog>

#include "typedefs.h"

namespace Ui {
class DifferentialFunctionDialog;
}

class DifferentialFunction;

class DifferentialFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DifferentialFunctionDialog(QWidget *parent = 0);
    explicit DifferentialFunctionDialog(const DifferentialFunctionPtr& f, QWidget *parent = 0);
    ~DifferentialFunctionDialog();

    void setFunction(const DifferentialFunctionPtr& f);

public Q_SLOTS:
    void accept();
    void selectFunction();

private:
    void init();
    void functionSelected();

private:
    Ui::DifferentialFunctionDialog *ui;


    DifferentialFunctionPtr mFunction;

    bool mEdit;
};

#endif // DIFFERENTIALCURVEDIALOG_H
