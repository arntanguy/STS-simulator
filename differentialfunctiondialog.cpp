#include "differentialfunctiondialog.h"
#include "ui_differentialfunctiondialog.h"

#include "functionssingleton.h"

#include "differentialfunction.h"
#include "integralfunction.h"

#include "functionselectiondialog.h"

#include <QMessageBox>
#include <QDebug>

DifferentialFunctionDialog::DifferentialFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DifferentialFunctionDialog)
{
    ui->setupUi(this);

    mFunction = new DifferentialFunction();

    init();

}
DifferentialFunctionDialog::DifferentialFunctionDialog(DifferentialFunction *f, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DifferentialFunctionDialog)
{
    ui->setupUi(this);
    init();

    if(f != 0) {
        setFunction(f);
    } else {
        mFunction = new DifferentialFunction();
    }

}

void DifferentialFunctionDialog::init()
{
    mEdit = false;
    connect(ui->functionSelect, SIGNAL(clicked()), this, SLOT(selectFunction()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

void DifferentialFunctionDialog::setFunction(DifferentialFunction* f)
{
    mEdit = true;
    mFunction = f;
    ui->differentialName->setText(mFunction->getName());
    functionSelected();
}

DifferentialFunctionDialog::~DifferentialFunctionDialog()
{
    delete ui;
}


void DifferentialFunctionDialog::functionSelected()
{
    ui->functionExpression->setText(mFunction->getExpression());
}

void DifferentialFunctionDialog::selectFunction()
{
    if(mFunction == 0) {
        mFunction = new DifferentialFunction();
    }
    FunctionSelectionDialog dialog(this, AbstractFunction::Integral);
    Function *f = 0;
    if(dialog.exec() == QDialog::Accepted) {
        // Do not copy as we're linking the real funciton to the derivate
        f = dialog.getSelectedFunction();
    }
    IntegralFunction *intF = dynamic_cast<IntegralFunction *>(f);
    if(intF != 0) {
        mFunction->setFunction(intF);
        functionSelected();
    }
}

void DifferentialFunctionDialog::accept()
{
    qDebug() << "DifferentialFunctionDialog::accept()";
    bool mayClose = false;
    if(!mEdit && Singleton<FunctionsSingleton>::Instance().functionNameExists(ui->differentialName->text())) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Name Conflict"), tr("Another curve with the name ") + ui->differentialName->text() + tr(" already exists. Do you want to modify the name?"), QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            mayClose = false;
            return;
        } else {
            mayClose = true;
        }
    } else {
        mayClose = true;
    }
    if(mayClose) {
        if(mFunction != 0) {
            mFunction->setName(ui->differentialName->text());
            Singleton<FunctionsSingleton>::Instance().addFunction(mFunction);
        }
        QDialog::accept();
    }
}
