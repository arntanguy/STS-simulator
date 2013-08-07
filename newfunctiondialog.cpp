#include "newfunctiondialog.h"
#include "ui_newfunctiondialog.h"
#include "function.h"
#include "functionssingleton.h"
#include "functionvariableswidget.h"
#include "globalsettingssingleton.h"

#include <QMessageBox>
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
    if(f != 0) {
        setWindowTitle("Variables of " + f->getName());
    }
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

    mSettings = &Singleton<GlobalSettingsSingleton>::Instance();
    connect(mSettings, SIGNAL(overlayOpacityUpdated()), this, SLOT(updateOpacity()));
    setWindowOpacity(mSettings->getOverlayOpacity());
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

bool NewFunctionDialog::setupFunction()
{
    FunctionsSingleton *singleton = &Singleton<FunctionsSingleton>::Instance();
    if(mFunction == 0)
        mFunction = new Function();
    mFunction->setName(ui->functionName->text());
    mFunction->setExpression(ui->functionExpression->toPlainText());
    if(mFunction->isValidExpression()) {
        qDebug() << "Valid expression, accept";
        if(!mEditFunction) {
            //if(singleton->hasFunction(ui->functionName->text())) {
            //    QMessageBox::critical(this, tr("Function name already exists"),
            //            tr("You can't have two base functions with the same name!"));
            //    return false;
            //}
            singleton->addFunction(mFunction);
            setFunction(mFunction);
        }
        return true;
    } else {
        QMessageBox::critical(this, tr("Function expression is invalid"),
                tr("The function entered contains the following error:\n\n")+mFunction->getError());
        return false;
    }
}

// =============================== PUBLIC =================================
void NewFunctionDialog::setFunction(Function *f)
{
    ui->functionName->setText(mFunction->getName());
    ui->functionExpression->setText(mFunction->getExpression());
    ui->variablesWidget->setFunction(mFunction);
    mEditFunction = true;
}

// =============================== SLOTS ==================================
void NewFunctionDialog::accept()
{
    if(setupFunction()) {
        ui->variablesWidget->save();
        emit accepted();
        QDialog::accept();
    } else {
        QDialog::reject();
    }
}

void NewFunctionDialog::pageChanged(int index)
{
    if(index == 1) {
        setupFunction();
    }
}

void NewFunctionDialog::updateOpacity()
{
    setWindowOpacity(mSettings->getOverlayOpacity());
}
