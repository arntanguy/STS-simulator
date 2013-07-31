#ifndef HIERARCHICALFUNCTIONDIALOG_H
#define HIERARCHICALFUNCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class HierarchicalFunctionDialog;
}

class HierarchicalFunction;
class Function;

class HierarchicalFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HierarchicalFunctionDialog(QWidget *parent = 0);
    ~HierarchicalFunctionDialog();

    HierarchicalFunction* getFunction() {
        return mFunction;
    }
    void setFunction(HierarchicalFunction *f);

private:
    void init();
    void initFromFunction();
    void addFunction(Function *f);
    void addFunctionItem(Function *f);

public Q_SLOTS:
    void addFunction();
    void removeFunction();
    void accept();
    void updateExpression();

Q_SIGNALS:
    void expressionChanged();

private:
    Ui::HierarchicalFunctionDialog *ui;

    HierarchicalFunction *mFunction;
};

#endif // HIERACHICALFUNCTIONDIALOG_H
