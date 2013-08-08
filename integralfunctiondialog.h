#ifndef INTEGRALFUNCTIONDIALOG_H
#define INTEGRALFUNCTIONDIALOG_H

#include <QDialog>
#include <QModelIndex>

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

    IntegralFunction* getFunction() {
        return mFunction;
    }
    void setFunction(IntegralFunction *f);

private:
    void init();
    void initFromFunction();
    void addFunction(Function *function);
    void addFunctionItem(Function *);
    void useFunction(Function *);
    void setFunctionConfiguration(Function *f);

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

    IntegralFunction *mFunction;
    Function *mFunctionInUse;

    bool mEdit;
};

#endif // INTEGRALFUNCTIONDIALOG_H
