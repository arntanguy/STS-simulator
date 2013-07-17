#include "hierarchicalfunctiondialog.h"
#include "ui_hierarchicalfunctiondialog.h"
#include "functionselectiondialog.h"
#include "functionssingleton.h"
#include "function.h"
#include "hierarchicalfunction.h"
#include "functionfactory.h"

#include <QStandardItemModel>
#include <QDebug>

HierarchicalFunctionDialog::HierarchicalFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HierarchicalFunctionDialog)
{
    ui->setupUi(this);
    mFunction =  new HierarchicalFunction();
    init();
}

HierarchicalFunctionDialog::~HierarchicalFunctionDialog()
{
    delete ui;
}

void HierarchicalFunctionDialog::init()
{
    ui->functionView->setModel(new QStandardItemModel());
    connect(ui->functionAdd, SIGNAL(clicked()), this, SLOT(addFunction()));
    connect(ui->functionRemove, SIGNAL(clicked()), this, SLOT(removeFunction()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    connect(this, SIGNAL(expressionChanged()), this, SLOT(updateExpression()));
}

void HierarchicalFunctionDialog::setFunction(HierarchicalFunction *f)
{
    qDebug() << "HierarchicalFunctionDialog::setFunction("<<f->getName()<<")";
    mFunction = f;
    ui->functionName->setText(f->getName());
    setWindowTitle(tr("Edit Function ") + f->getName());
    if(f != 0) {
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
        model->clear();
        foreach(AbstractFunction* af, mFunction->getFunctions()) {
            addFunctionItem(af);
        }
    }
}

// ============================= PRIVATE =======================
void HierarchicalFunctionDialog::addFunction(AbstractFunction *f)
{
    if(f != 0) {
        addFunctionItem(f);
        mFunction->addFunction(f);

        emit expressionChanged();
    }
}

void HierarchicalFunctionDialog::addFunctionItem(AbstractFunction *f)
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
    FunctionSelectionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        // XXX: check if copy is allright
        addFunction(FunctionFactory::createFromFunction(dialog.getSelectedFunction()));
    }
}

void HierarchicalFunctionDialog::removeFunction()
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
    QModelIndex index = ui->functionView->currentIndex();
    AbstractFunction *f = index.data(Qt::UserRole).value<AbstractFunction *>();
    mFunction->removeFunction(f);
    model->removeRow(index.row());
    delete f;
    emit expressionChanged();
}


void HierarchicalFunctionDialog::accept()
{
    mFunction->setName(ui->functionName->text());
    Singleton<FunctionsSingleton>::Instance().addFunction(mFunction);

    QDialog::accept();
}

void HierarchicalFunctionDialog::updateExpression()
{
    ui->functionExpression->setText(mFunction->getExpression());
}
