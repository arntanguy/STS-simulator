#ifndef GLOBALCONFIGDIALOG_H
#define GLOBALCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class GlobalConfigDialog;
}

class GlobalSettingsSingleton;

class GlobalConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GlobalConfigDialog(QWidget *parent = 0);
    ~GlobalConfigDialog();

private:
    void init();

public Q_SLOTS:
    void accept();

private:
    Ui::GlobalConfigDialog *ui;

    GlobalSettingsSingleton *mSingleton;
};

#endif // GLOBALCONFIGDIALOG_H
