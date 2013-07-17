#include "newfunctiondialog.h"
#include "ui_newfunctiondialog.h"
#include "function.h"
#include "functionssingleton.h"
#include "functionvariableswidget.h"

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

// =============================== PRIVATE =================================
void NewFunctionDialog::init()
{
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)));

    if(mFunction != 0) {
        setFunction(mFunction);

        mEditFunction = true;
    } else {
        mEditFunction = false;
    }
}

bool NewFunctionDialog::setupFunction()
{
    if(mFunction == 0)
        mFunction = new Function();
    mFunction->setName(ui->functionName->text());
    mFunction->setExpression(ui->functionExpression->toPlainText());
    if(mFunction->isValidExpression()) {
        qDebug() << "Valid expression, accept";
        if(!mEditFunction)
            Singleton<FunctionsSingleton>::Instance().addFunction(mFunction);
        return true;
        setFunction(mFunction);
    } else {
        return false;
    }
}

// =============================== PUBLIC =================================
void NewFunctionDialog::setFunction(Function *f)
{
    ui->functionName->setText(mFunction->getName());
    ui->functionExpression->setText(mFunction->getExpression());
    ui->variablesWidget->setFunction(mFunction);
}

// =============================== SLOTS ==================================
void NewFunctionDialog::accept()
{
    if(setupFunction()) {
        ui->variablesWidget->save(mFunction->getGroup());
        QDialog::accept();
    } else {
        QDialog::reject();
        qDebug() << "Invalid expression, ask";
    }
}

void NewFunctionDialog::pageChanged(int)
{
    setupFunction();
}
