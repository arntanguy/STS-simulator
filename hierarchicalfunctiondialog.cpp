#include "hierarchicalfunctiondialog.h"
#include "ui_hierarchicalfunctiondialog.h"
#include "functionselectiondialog.h"
#include "functionssingleton.h"
#include "function.h"
#include "hierarchicalfunction.h"

#include <QStandardItemModel>

HierarchicalFunctionDialog::HierarchicalFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HierarchicalFunctionDialog)
{
    ui->setupUi(this);
    ui->functionView->setModel(new QStandardItemModel());
    connect(ui->functionAdd, SIGNAL(clicked()), this, SLOT(addFunction()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    mFunction =  new HierarchicalFunction();
}

HierarchicalFunctionDialog::~HierarchicalFunctionDialog()
{
    delete ui;
}

void HierarchicalFunctionDialog::setFunction(HierarchicalFunction *f)
{
    mFunction = f;
    if(f != 0) {
        foreach(AbstractFunction* af, mFunction->getFunctions()) {
            addFunction(af);
        }
    }
}

// ============================= PRIVATE =======================
void HierarchicalFunctionDialog::addFunction(AbstractFunction *f)
{
    if(f != 0) {
        QStandardItem *item = new QStandardItem();
        item->setText(f->getName());
        item->setData(QVariant::fromValue(f), Qt::UserRole);
        mFunction->addFunction(f);
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->functionView->model());
        model->setItem(model->rowCount(), item);
    }
}

// ============================= SLOTS ==========================

void HierarchicalFunctionDialog::addFunction()
{
    FunctionSelectionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        addFunction(dialog.getSelectedFunction());
    }
}

void HierarchicalFunctionDialog::accept()
{
    mFunction->setName(ui->functionName->text());
    Singleton<FunctionsSingleton>::Instance().addFunction(mFunction);

    QDialog::accept();
}
