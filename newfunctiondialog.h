#ifndef NEWFUNCTIONDIALOG_H
#define NEWFUNCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class NewFunctionDialog;
}

class NewFunctionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewFunctionDialog(QWidget *parent = 0);
    ~NewFunctionDialog();
    
private:
    Ui::NewFunctionDialog *ui;
};

#endif // NEWFUNCTIONDIALOG_H
