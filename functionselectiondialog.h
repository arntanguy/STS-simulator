#ifndef FUNCTIONSELECTIONDIALOG_H
#define FUNCTIONSELECTIONDIALOG_H

#include <QDialog>
#include "abstractfunction.h"

class QModelIndex;
class Function;
class QStandardItem;

namespace Ui {
class FunctionSelectionDialog;
}


class FunctionSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionSelectionDialog(QWidget *parent = 0, AbstractFunction::FunctionType flags = AbstractFunction::All);
    ~FunctionSelectionDialog();

    Function* getSelectedFunction();

private:
    void init();
    void useFunction(Function *);
    QStandardItem* createItem(Function *);

private:
    Ui::FunctionSelectionDialog *ui;
    Function *mCurrentFunction;
    AbstractFunction::FunctionType mFlags;

public Q_SLOTS:
    void functionSelected(const QModelIndex &);
    void variableValueChanged(QString, double);
    void newFunction();
    void accept();
};

#endif // NEWFUNCTIONDIALOG_H
