#include "newfunctiondialog.h"
#include "ui_newfunctiondialog.h"
#include "function.h"
#include "functionssingleton.h"
#include "functionvariableswidget.h"
#include "globalsettingssingleton.h"
#include "functionhelpdialog.h"

#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

NewFunctionDialog::NewFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFunctionDialog)
{
    ui->setupUi(this);

    init();
}

NewFunctionDialog::NewFunctionDialog(FunctionPtr &f, QWidget *parent) :
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

    connect(ui->functionHelp, SIGNAL(clicked()), this, SLOT(help()));

    setWindowOpacity(mSettings->getOverlayOpacity());
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

}

bool NewFunctionDialog::setupFunction()
{
    FunctionsSingleton *singleton = &Singleton<FunctionsSingleton>::Instance();
    if(mFunction == 0)
        mFunction = FunctionPtr(new Function());
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
            ui->variablesWidget->save();
        }
        return true;
    } else {
        QMessageBox::critical(this, tr("Function expression is invalid"),
                tr("The function entered contains the following error:\n\n")+mFunction->getError());
        return false;
    }
}

// =============================== PUBLIC =================================
void NewFunctionDialog::setFunction(const FunctionPtr &f)
{
    ui->functionName->setText(mFunction->getName());
    ui->functionExpression->setText(mFunction->getExpression());
    ui->variablesWidget->setFunction(mFunction);
    mEditFunction = true;
}


/// =================== VIRTUAL REIMPLEMENTED ===========================
void NewFunctionDialog::keyPressEvent(QKeyEvent *evt)
{
    if(evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(evt);
}

// =============================== SLOTS ==================================
void NewFunctionDialog::accept()
{
    bool mMayClose = false;
    if(!mEditFunction && Singleton<FunctionsSingleton>::Instance().functionNameExists(ui->functionName->text())) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Name Conflict"), tr("Another curve with the name ") + ui->functionName->text() + tr(" already exists. Do you want to modify the name?"), QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            mMayClose = false;
            return;
        } else {
            mMayClose = true;
        }
    } else {
        mMayClose = true;
    }
    if(mMayClose && mFunction != 0) {
        qDebug() << "SAVE";
        ui->variablesWidget->save();
        emit accepted();
        QDialog::accept();
    }
}

void NewFunctionDialog::pageChanged(int index)
{
    if(index == 1) {
        setupFunction();
    } else {
        ui->variablesWidget->saveIfNeeded();
    }
}

void NewFunctionDialog::updateOpacity()
{
    setWindowOpacity(mSettings->getOverlayOpacity());
}

void NewFunctionDialog::help()
{
    FunctionHelpDialog dialog(this);
    dialog.exec();
}
