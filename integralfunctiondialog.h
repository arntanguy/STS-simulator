#ifndef INTEGRALFUNCTIONDIALOG_H
#define INTEGRALFUNCTIONDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include "typedefs.h"

namespace Ui {
class IntegralFunctionDialog;
}

class IntegralFunction;
class Function;

class IntegralFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IntegralFunctionDialog(QWidget *parent = 0);
    ~IntegralFunctionDialog();

    IntegralFunctionPtr getFunction() {
        return mFunction;
    }
    void setFunction(const IntegralFunctionPtr &f);

private:
    void init();
    void initTransmission();
    void initFromFunction();
    void addFunction(const FunctionPtr &function);
    void addFunctionItem(const FunctionPtr &);
    void useFunction(const FunctionPtr &);
    void setFunctionConfiguration(const FunctionPtr &f);

Q_SIGNALS:
    void expressionChanged();

public Q_SLOTS:
    void addFunction();
    void removeFunction();
    void accept();
    void updateExpression();
    void functionSelectionChanged( const QModelIndex & , const QModelIndex & );
    void parametersEdited();
    void integrationVariableEdited();

private:
    Ui::IntegralFunctionDialog *ui;

    IntegralFunctionPtr mFunction;
    FunctionPtr mFunctionInUse;

    bool mEdit;
};

#endif // INTEGRALFUNCTIONDIALOG_H
