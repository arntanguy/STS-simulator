#include "functionselectiondialog.h"
#include "ui_functionselectiondialog.h"
#include "newfunctiondialog.h"
#include "functionssingleton.h"
#include "function.h"
#include "valueselector.h"
#include "variablefactory.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <QModelIndex>

FunctionSelectionDialog::FunctionSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FunctionSelectionDialog)
{
    ui->setupUi(this);
    init();

    connect(ui->functionList, SIGNAL(clicked ( const QModelIndex &)), this, SLOT(functionSelected(const QModelIndex &)));
    connect(ui->newFunctionButton, SIGNAL(clicked()), this, SLOT(newFunction()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

FunctionSelectionDialog::~FunctionSelectionDialog()
{
    delete ui;
}

void FunctionSelectionDialog::init()
{
    FunctionsSingleton *singleton = &Singleton<FunctionsSingleton>::Instance();
    QStringList fNames = singleton->getFunctionNames();
    QStandardItemModel *model = new QStandardItemModel();
    ui->functionList->setModel(model);
    if(model != 0) {
        int row = 0;
        foreach(QString name, fNames) {
            Function *f = dynamic_cast<Function*>(singleton->getFunction(name));
            if(f != 0)
                model->setItem(row++, createItem(f));
        }
    } else {
        qDebug() << "null model";
    }
}

Function* FunctionSelectionDialog::getSelectedFunction()
{
    return mCurrentFunction;
}


// ======================= PRIVATE ========================================
void FunctionSelectionDialog::useFunction(Function *f)
{
    mCurrentFunction = f;
    qDebug() << "Selected function " << f->getName();
    ui->functionExpression->setText("<b>"+f->getName()+ "</b>(" + f->getVariable() + ") = " + f->getExpression());
    QStringList variables = f->getVariableFactory()->getVariableNames();
    qDebug() << "Function variables: " << variables;
    foreach(QString var, variables) {
        // If it's not the function variable
        if(var != f->getVariable()) {
            ValueSelector *valueSelector = new ValueSelector(var, f, this);
            connect(valueSelector, SIGNAL(valueChanged(QString,double)), this, SLOT(variableValueChanged(QString, double)));
            // Creates a widget to control it
            ui->functionVariablesLayout->addWidget(valueSelector);
        }
    }
}

QStandardItem* FunctionSelectionDialog::createItem(Function *f)
{
    QStandardItem *item = new QStandardItem();
    item->setText(f->getName());
    item->setData(QVariant::fromValue<Function *>(f), Qt::UserRole);
    return item;
}

// =============================== SLOTS ==================================
void FunctionSelectionDialog::functionSelected(const QModelIndex &index)
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->functionList->model());
    if(model != 0) {
        QVariant item = model->data(index, Qt::UserRole);
        if(item.isValid()) {
            qDebug() << item;
            Function *f = dynamic_cast<Function *>(item.value<Function *>());
            if(f != 0) {
                useFunction(f);
            } else {
                qDebug() << "NULL FUNCTION!!!!";
            }
        }
    }
}

void FunctionSelectionDialog::variableValueChanged(QString var, double val)
{
    qDebug() << "Variable value changed: " << var << ": " << val;
    if(mCurrentFunction != 0) {
        qDebug() << "Eval: " << mCurrentFunction->getExpression() << " for value V=2 " << mCurrentFunction->compute(2);
    }
}

void FunctionSelectionDialog::newFunction()
{
    NewFunctionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionList->model());
        if(model != 0) {
            Function *f=dialog.getFunction();
            if(f != 0) {
                model->setItem(model->rowCount(), createItem(f));
            }
        }
    }
}

void FunctionSelectionDialog::accept()
{
    QDialog::accept();
}
