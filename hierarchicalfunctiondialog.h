#ifndef HIERARCHICALFUNCTIONDIALOG_H
#define HIERARCHICALFUNCTIONDIALOG_H

#include <QDialog>
#include "qthideandshowdialog.h"
#include "typedefs.h"

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

    HierarchicalFunctionPtr getFunction() {
        return mFunction;
    }
    void setFunction(const HierarchicalFunctionPtr& f);

private:
    void init();
    void initFromFunction();
    void addFunction(const FunctionPtr& f);
    void addFunctionItem(const FunctionPtr& f);

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

    HierarchicalFunctionPtr mFunction;

    bool mEdit;
};

#endif // HIERACHICALFUNCTIONDIALOG_H
