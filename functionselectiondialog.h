#ifndef FUNCTIONSELECTIONDIALOG_H
#define FUNCTIONSELECTIONDIALOG_H

#include <QDialog>
#include "abstractfunction.h"
#include "typedefs.h"

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

    FunctionPtr getSelectedFunction();

private:
    void init();
    void useFunction(const FunctionPtr &);
    QStandardItem* createItem(const FunctionPtr &);

private:
    Ui::FunctionSelectionDialog *ui;
    FunctionPtr mCurrentFunction;
    AbstractFunction::FunctionType mFlags;

public Q_SLOTS:
    void functionSelected(const QModelIndex &);
    void variableValueChanged(QString, double);
    void newFunction();
    void accept();
};

#endif // NEWFUNCTIONDIALOG_H
