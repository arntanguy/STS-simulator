#include "differentialfunctiondialog.h"
#include "ui_differentialfunctiondialog.h"

#include "functionssingleton.h"

#include "differentialfunction.h"
#include "integralfunction.h"


#include "functionselectiondialog.h"

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

    if(f != 0) {
        setFunction(f);
    } else {
        mFunction = new DifferentialFunction();
    }

    init();
}

void DifferentialFunctionDialog::init()
{
    connect(ui->functionSelect, SIGNAL(clicked()), this, SLOT(selectFunction()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void DifferentialFunctionDialog::setFunction(DifferentialFunction* f)
{
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
    if(mFunction != 0) {
        mFunction->setName(ui->differentialName->text());
        Singleton<FunctionsSingleton>::Instance().addFunction(mFunction);
    }
    QDialog::accept();
}
