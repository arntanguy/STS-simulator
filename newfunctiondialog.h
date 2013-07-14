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
    explicit NewFunctionDialog(Function *f, QWidget *parent = 0);
    ~NewFunctionDialog();

    Function *getFunction() {
        return mFunction;
    }

private:
    void init();

public slots:
    void accept();

private:
    Ui::NewFunctionDialog *ui;
    Function *mFunction;
    bool mEditFunction;
};

#endif // NEWFUNCTIONDIALOG_H
