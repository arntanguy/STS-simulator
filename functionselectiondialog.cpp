#include "functionselectiondialog.h"
#include "ui_functionselectiondialog.h"
#include "functionssingleton.h"
#include "function.h"
#include "valueselector.h"

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
            QStandardItem *item = new QStandardItem();
            item->setText(name);
            item->setData(QVariant::fromValue<Function *>(singleton->getFunction(name)), Qt::UserRole);
            model->setItem(row++, item);
        }
    } else {
        qDebug() << "null model";
    }
}

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
            ValueSelector *valueSelector = new ValueSelector(var, f->getVariableFactory()->getVariableAddress(var), this);
            connect(valueSelector, SIGNAL(valueChanged(QString,double)), this, SLOT(variableValueChanged(QString, double)));
            // Creates a widget to control it
            ui->functionVariablesLayout->addWidget(valueSelector);
        }
    }
}

// =============================== SLOTS ==================================
void FunctionSelectionDialog::functionSelected(const QModelIndex &index)
{
    qDebug() << "Function selected";

    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->functionList->model());
    if(model != 0) {
        QVariant item = model->data(index, Qt::UserRole);
        if(item.isValid()) {
            qDebug() << item;
            Function *f = static_cast<Function *>(item.value<Function *>());
            if(f != 0) {
                useFunction(f);
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
