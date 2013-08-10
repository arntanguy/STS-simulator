#include "hierarchicalfunctiondialog.h"
#include "ui_hierarchicalfunctiondialog.h"
#include "functionselectiondialog.h"
#include "functionssingleton.h"
#include "function.h"
#include "hierarchicalfunction.h"
#include "functionfactory.h"

#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>

HierarchicalFunctionDialog::HierarchicalFunctionDialog(QWidget *parent) :
    QtHideAndShowDialog(parent),
    ui(new Ui::HierarchicalFunctionDialog)
{
    ui->setupUi(this);
    mFunction =  HierarchicalFunctionPtr(new HierarchicalFunction());
    init();
}

HierarchicalFunctionDialog::~HierarchicalFunctionDialog()
{
    delete ui;
}

void HierarchicalFunctionDialog::init()
{
    mEdit = false;

    ui->functionView->setModel(new QStandardItemModel());
    connect(ui->functionAdd, SIGNAL(clicked()), this, SLOT(addFunction()));
    connect(ui->functionRemove, SIGNAL(clicked()), this, SLOT(removeFunction()));


    connect(this, SIGNAL(expressionChanged()), this, SLOT(updateExpression()));
}

void HierarchicalFunctionDialog::setFunction(const HierarchicalFunctionPtr& f)
{
    qDebug() << "HierarchicalFunctionDialog::setFunction("<<f->getName()<<")";
    mEdit = true;
    mFunction = f;
    ui->functionName->setText(f->getName());
    setWindowTitle(tr("Edit Function ") + f->getName());
    if(f != 0) {
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
        model->clear();
        foreach(QSharedPointer<Function> af, mFunction->getFunctions()) {
            addFunctionItem(af);
        }
    }
}

// ============================= PRIVATE =======================
void HierarchicalFunctionDialog::addFunction(const FunctionPtr& f)
{
    if(f != 0) {
        addFunctionItem(f);
        mFunction->addFunction(f);

        emit expressionChanged();
    }
}

void HierarchicalFunctionDialog::addFunctionItem(const FunctionPtr& f)
{
    if(f != 0) {
        QStandardItem *item = new QStandardItem();
        item->setText(f->getName());
        item->setData(QVariant::fromValue(f), Qt::UserRole);
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
        model->setItem(model->rowCount(), item);

        emit expressionChanged();
    }
}

// ============================= SLOTS ==========================

void HierarchicalFunctionDialog::addFunction()
{
    FunctionSelectionDialog dialog(this, AbstractFunction::Function);
    if(dialog.exec() == QDialog::Accepted) {
        // Add a copy the function
        addFunction(FunctionFactory::createFromFunction(dialog.getSelectedFunction()));
    }
}

void HierarchicalFunctionDialog::removeFunction()
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
    QModelIndex index = ui->functionView->currentIndex();
    FunctionPtr f = index.data(Qt::UserRole).value<FunctionPtr>();
    mFunction->removeFunction(f);
    model->removeRow(index.row());
    emit expressionChanged();
}


void HierarchicalFunctionDialog::accept()
{
    bool mayClose = false;
    if(!mEdit && Singleton<FunctionsSingleton>::Instance().functionNameExists(ui->functionName->text())) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Name Conflict"), tr("Another curve with the name ") + ui->functionName->text() + tr(" already exists. Do you want to modify the name?"), QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            mayClose = false;
            return;
        } else {
            mayClose = true;
        }
    } else {
        mayClose = true;
    }
    mFunction->setName(ui->functionName->text());
    Singleton<FunctionsSingleton>::Instance().addFunction(mFunction);

    if(mayClose)
        QDialog::accept();
}

void HierarchicalFunctionDialog::updateExpression()
{
    ui->functionExpression->setText(mFunction->getExpression());
}
