#ifndef NEWFUNCTIONDIALOG_H
#define NEWFUNCTIONDIALOG_H

#include <QDialog>
#include "qthideandshowdialog.h"

namespace Ui {
class NewFunctionDialog;
}

class Function;
class GlobalSettingsSingleton;

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
    void setFunction(Function *f);

private:
    void init();
    bool setupFunction();

Q_SIGNALS:
    void accepted();

public Q_SLOTS:
    void accept();
    void pageChanged(int);
    void updateOpacity();
    void help();

private:
    Ui::NewFunctionDialog *ui;
    Function *mFunction;
    bool mEditFunction;

    GlobalSettingsSingleton *mSettings;
};

#endif // NEWFUNCTIONDIALOG_H
