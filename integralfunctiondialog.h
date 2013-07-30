#ifndef INTEGRALFUNCTIONDIALOG_H
#define INTEGRALFUNCTIONDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class IntegralFunctionDialog;
}

class IntegralFunction;
class AbstractFunction;
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
    void addFunction(AbstractFunction *function);
    void addFunctionItem(AbstractFunction *);
    void useFunction(AbstractFunction *);
    void setFunctionConfiguration(Function *f);

Q_SIGNALS:
    void expressionChanged();

public Q_SLOTS:
    void addFunction();
    void removeFunction();
    void accept();
    void updateExpression();
//    void functionSelected(const QModelIndex &index);
    void functionSelectionChanged( const QModelIndex & , const QModelIndex & );

private:
    Ui::IntegralFunctionDialog *ui;

    IntegralFunction *mFunction;
};

#endif // INTEGRALFUNCTIONDIALOG_H
