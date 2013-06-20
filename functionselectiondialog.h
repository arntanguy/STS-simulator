#ifndef NEWFUNCTIONDIALOG_H
#define NEWFUNCTIONDIALOG_H

#include <QDialog>

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
    Ui::FunctionSelectionDialog *ui;
};

#endif // NEWFUNCTIONDIALOG_H
