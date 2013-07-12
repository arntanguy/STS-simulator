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

    mFunction = 0;
    init();
}

NewFunctionDialog::NewFunctionDialog(Function *f, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFunctionDialog)
{
    ui->setupUi(this);
    mFunction = f;
    init();
}

NewFunctionDialog::~NewFunctionDialog()
{
    delete ui;
}

void NewFunctionDialog::init()
{
    if(mFunction != 0) {
        ui->functionName->setText(mFunction->getName());
        ui->functionExpression->setText(mFunction->getExpression());
    }
}

// =============================== SLOTS ==================================
void NewFunctionDialog::accept()
{
    if(mFunction == 0)
        mFunction = new Function();
    mFunction->setName(ui->functionName->text());
    mFunction->setExpression(ui->functionExpression->toPlainText());
    if(mFunction->isValidExpression()) {
        qDebug() << "Valid expression, accept";
        Singleton<FunctionsSingleton>::Instance().addFunction(mFunction);
        QDialog::accept();
    } else {
        QDialog::reject();
        qDebug() << "Invalid expression, ask";
    }

}
