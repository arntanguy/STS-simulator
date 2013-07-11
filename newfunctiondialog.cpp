#include "newfunctiondialog.h"
#include "ui_newfunctiondialog.h"
#include "function.h"
#include "functionssingleton.h"

#include <QDebug>

NewFunctionDialog::NewFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFunctionDialog)
{
    ui->setupUi(this);
}

NewFunctionDialog::~NewFunctionDialog()
{
    delete ui;
}


// =============================== SLOTS ==================================
void NewFunctionDialog::accept()
{
    Function *f = new Function();
    f->setName(ui->functionName->text());
    f->setExpression(ui->functionExpression->toPlainText());
    if(f->isValidExpression()) {
        qDebug() << "Valid expression, accept";
        Singleton<FunctionsSingleton>::Instance().addFunction(f);
        mFunction = f;
        QDialog::accept();
    } else {
        QDialog::reject();
        qDebug() << "Invalid expression, ask";
    }

}
