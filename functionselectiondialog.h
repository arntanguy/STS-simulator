#ifndef FUNCTIONSELECTIONDIALOG_H
#define FUNCTIONSELECTIONDIALOG_H

#include <QDialog>

class QModelIndex;
class Function;

namespace Ui {
class FunctionSelectionDialog;
}

class FunctionSelectionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FunctionSelectionDialog(QWidget *parent = 0);
    ~FunctionSelectionDialog();

private:
    void init();
    void useFunction(Function *);

private:
    Ui::FunctionSelectionDialog *ui;
    Function *mCurrentFunction;

public slots:
    void functionSelected(const QModelIndex &);
    void variableValueChanged(QString, double);
};

#endif // NEWFUNCTIONDIALOG_H
