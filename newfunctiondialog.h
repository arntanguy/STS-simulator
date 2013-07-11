#ifndef NEWFUNCTIONDIALOG_H
#define NEWFUNCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class NewFunctionDialog;
}

class Function;

class NewFunctionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewFunctionDialog(QWidget *parent = 0);
    ~NewFunctionDialog();
    
    Function *getFunction() {
        return mFunction;
    }

private:
    Ui::NewFunctionDialog *ui;
    Function *mFunction;

public slots:
    void accept();
};

#endif // NEWFUNCTIONDIALOG_H
