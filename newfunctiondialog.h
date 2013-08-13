#ifndef NEWFUNCTIONDIALOG_H
#define NEWFUNCTIONDIALOG_H

#include <QDialog>
#include "qthideandshowdialog.h"
#include "typedefs.h"

namespace Ui {
class NewFunctionDialog;
}

class QKeyEvent;
class Function;
class GlobalSettingsSingleton;

class NewFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFunctionDialog(QWidget *parent = 0);
    explicit NewFunctionDialog(FunctionPtr &f, QWidget *parent = 0);
    ~NewFunctionDialog();

    FunctionPtr getFunction() {
        return mFunction;
    }
    void setFunction(const FunctionPtr &f);

private:
    void init();
    bool setupFunction();

protected:
    virtual void keyPressEvent(QKeyEvent *evt);

Q_SIGNALS:
    void accepted();

public Q_SLOTS:
    void accept();
    void pageChanged(int);
    void updateOpacity();
    void help();

private:
    Ui::NewFunctionDialog *ui;
    FunctionPtr mFunction;
    bool mEditFunction;

    GlobalSettingsSingleton *mSettings;
};

#endif // NEWFUNCTIONDIALOG_H
