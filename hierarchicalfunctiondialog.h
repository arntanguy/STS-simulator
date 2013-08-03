#ifndef HIERARCHICALFUNCTIONDIALOG_H
#define HIERARCHICALFUNCTIONDIALOG_H

#include <QDialog>
#include "qthideandshowdialog.h"

namespace Ui {
class HierarchicalFunctionDialog;
}

class HierarchicalFunction;
class Function;

class HierarchicalFunctionDialog : public QtHideAndShowDialog
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
    void accepted();

private:
    Ui::HierarchicalFunctionDialog *ui;

    HierarchicalFunction *mFunction;
};

#endif // HIERACHICALFUNCTIONDIALOG_H
