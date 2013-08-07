#ifndef FUNCTIONHELPDIALOG_H
#define FUNCTIONHELPDIALOG_H

#include <QDialog>

namespace Ui {
class FunctionHelpDialog;
}

namespace mu {
    class Parser;
}

class FunctionHelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionHelpDialog(QWidget *parent = 0);
    ~FunctionHelpDialog();

private:
    void init();

private:
    Ui::FunctionHelpDialog *ui;

    mu::Parser *mParser;
};

#endif // FUNCTIONHELPDIALOG_H
