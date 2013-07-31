#include "integralfunctiondialog.h"
#include "ui_integralfunctiondialog.h"
#include "integralfunction.h"
#include "variablefactory.h"
#include "functionselectiondialog.h"
#include "functionssingleton.h"

#include <QStandardItemModel>
#include <QDebug>


IntegralFunctionDialog::IntegralFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntegralFunctionDialog)
{
    ui->setupUi(this);

    mFunction = new IntegralFunction();

    init();
}

IntegralFunctionDialog::~IntegralFunctionDialog()
{
    delete ui;
}

void IntegralFunctionDialog::init()
{
    ui->integralView->setModel(new QStandardItemModel());

    ui->integralRange->addItem(tr("0 to V"), IntegralFunction::ZeroToV);
    ui->integralRange->addItem(tr("-V to 0"), IntegralFunction::MinusVToZero);

    connect(ui->integralAddFunction, SIGNAL(clicked()), this, SLOT(addFunction()));
    connect(ui->integralRemoveFunction, SIGNAL(clicked()), this, SLOT(removeFunction()));
    //connect(ui->integralView, SIGNAL(clicked ( const QModelIndex &)), this, SLOT(functionSelected(const QModelIndex &)));
    connect(ui->integralView->selectionModel(), SIGNAL(currentChanged (const QModelIndex & , const QModelIndex & )), this, SLOT(functionSelectionChanged(const QModelIndex& , const QModelIndex& )));
    connect(this, SIGNAL(expressionChanged()), this, SLOT(updateExpression()));


    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

void IntegralFunctionDialog::setFunction(IntegralFunction *f)
{
    qDebug() << "IntegralFunctionDialog::setFunction("<<f->getName()<<")";
    mFunction = f;
    ui->integralName->setText(f->getName());
    ui->integralIntegrationVariable->setText(f->getIntegrationVariable());

    int index = ui->integralRange->findData(f->getRange());
    if(index != -1)
        ui->integralRange->setCurrentIndex(index);

    setWindowTitle(tr("Edit IntegralFunction ") + f->getName());
    if(f != 0) {
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->integralView->model());
        model->clear();
        foreach(Function* af, mFunction->getFunctions()) {
            addFunctionItem(af);
        }
    }
}

// ============================= PRIVATE =======================
void IntegralFunctionDialog::addFunction(Function *f)
{
    if(f != 0) {
        f->setParameters(mFunction->getIntegrationVariable());
        addFunctionItem(f);
        mFunction->addFunction(f);

        emit expressionChanged();
    } else {
        //XXX
        qDebug() << "IntegralFunctionDialog::addFunctionItem: ERROR INVALID FUNCTION TYPE";
    }

}

void IntegralFunctionDialog::addFunctionItem(Function *f)
{
    if(f != 0) {
        QStandardItem *item = new QStandardItem();
        item->setText(f->getName());
        item->setData(QVariant::fromValue(f), Qt::UserRole);
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->integralView->model());
        model->setItem(model->rowCount(), item);

        emit expressionChanged();
    }  else {
        //XXX
        qDebug() << "IntegralFunctionDialog::addFunctionItem: ERROR INVALID FUNCTION TYPE";
    }
}

void IntegralFunctionDialog::setFunctionConfiguration(Function *f)
{
    qDebug() << "Set function configuration for "<< f->getName();
    QString parameter = ui->functionParameters->text();
    qDebug() << "with parameters: "<<parameter;
    if(parameter.isEmpty())
        f->setParameters(ui->integralIntegrationVariable->text());
    else
        f->setParameters(parameter);
}

void IntegralFunctionDialog::useFunction(Function *f)
{
    if(f != 0) {
        ui->functionParameters->setText(f->getParameters());
        ui->functionExpression->setText("<b>"+f->getName()+ "</b>(" + f->getVariable() + ") = " + f->getExpression());
    } else {
        //XXX:
        qDebug() << "IntegralFunctionDialog::useFunction: ERROR, INVALID FUNCTION TYPE";
    }
}

// ============================= SLOTS ==========================

void IntegralFunctionDialog::addFunction()
{
    FunctionSelectionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        // Don't copy for integral, it has to be updated to the parent curve
        // XXX: link any change to parent to integral
        addFunction(dialog.getSelectedFunction());
    }
}

void IntegralFunctionDialog::removeFunction()
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->integralView->model());
    QModelIndex index = ui->integralView->currentIndex();
    Function *f = index.data(Qt::UserRole).value<Function *>();
    // Detach function, don't delete it (linked to other function used elsewhere)!
    mFunction->removeFunction(f);
    model->removeRow(index.row());
    emit expressionChanged();
}


void IntegralFunctionDialog::accept()
{
    qDebug() << "IntegralFunctionDialog::accept()";
    mFunction->setName(ui->integralName->text());
    mFunction->setRange(static_cast<IntegralFunction::Range>(ui->integralRange->itemData(ui->integralRange->currentIndex()).toUInt()));

    // Save current function parameters
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->integralView->model());
    if(model != 0) {
        QModelIndex index = ui->integralView->currentIndex();
            QVariant item = model->data(index, Qt::UserRole);
            if(item.isValid()) {
                Function *f = item.value<Function*>();
                if(f != 0) {
                    setFunctionConfiguration(f);
                } else {
                    qDebug() << "invalid funciton";
                }
            } else {
                qDebug() << "invalid item";
            }
    }else {
        qDebug() << "null model";
    }

    Singleton<FunctionsSingleton>::Instance().addFunction(mFunction);

    QDialog::accept();
}

void IntegralFunctionDialog::updateExpression()
{
    ui->integralExpression->setText(mFunction->getExpression());
}

void IntegralFunctionDialog::functionSelectionChanged( const QModelIndex & newSelection, const QModelIndex & previousSelection)
{
    qDebug() << "################# functionSelectionChanged";
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->integralView->model());
    if(model != 0) {
            QVariant item = model->data(previousSelection, Qt::UserRole);
            if(item.isValid()) {
                Function *f = item.value<Function*>();
                if(f != 0) {
                    qDebug() << "Previous: " << f->getName();
                    setFunctionConfiguration(f);
                }
            }

            item = model->data(newSelection, Qt::UserRole);
            if(item.isValid()) {
                Function *f = item.value<Function*>();
                if(f != 0) {
                    qDebug() << "New: " << f->getName();
                    useFunction(f);
                }
            }
    }
}
