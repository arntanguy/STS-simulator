#include "integralfunctiondialog.h"
#include "ui_integralfunctiondialog.h"
#include "integralfunction.h"
#include "variablefactory.h"
#include "functionselectiondialog.h"
#include "functionssingleton.h"
#include "experimentalfunctionsingleton.h"
#include "transmissiondialog.h"

#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>


IntegralFunctionDialog::IntegralFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntegralFunctionDialog)
{
    ui->setupUi(this);

    mFunction = IntegralFunctionPtr(new IntegralFunction());

    init();
}

IntegralFunctionDialog::~IntegralFunctionDialog()
{
    delete ui;
}

void IntegralFunctionDialog::init()
{
    mEdit = false;
    ui->functionConfigWidget->hide();

    ui->integralView->setModel(new QStandardItemModel());

    ui->integralRange->addItem(tr("0 to V"), IntegralFunction::ZeroToV);
    ui->integralRange->addItem(tr("-V to 0"), IntegralFunction::MinusVToZero);

    initTransmission();

    connect(ui->integralAddFunction, SIGNAL(clicked()), this, SLOT(addFunction()));
    connect(ui->integralRemoveFunction, SIGNAL(clicked()), this, SLOT(removeFunction()));
    // TODO: use that one for the other function selection stuff
    connect(ui->integralView->selectionModel(), SIGNAL(currentChanged (const QModelIndex & , const QModelIndex & )), this, SLOT(functionSelectionChanged(const QModelIndex& , const QModelIndex& )));
    connect(ui->functionParameters, SIGNAL(editingFinished()), this, SLOT(parametersEdited()));
    connect(ui->integralIntegrationVariable, SIGNAL(editingFinished()), this, SLOT(integrationVariableEdited()));
    connect(ui->transmissionParameters, SIGNAL(clicked()), this, SLOT(configureTransmission()));

    connect(this, SIGNAL(expressionChanged()), this, SLOT(updateExpression()));
}

void IntegralFunctionDialog::initTransmission()
{
    // Load the available DZ function slots (defined at compile time)
    QMap<int, ExperimentalFunction *> functions = Singleton<ExperimentalFunctionSingleton>::Instance().getAllFunctions();
    foreach(int id, functions.keys()) {
        ui->DZFunction->addItem(functions[id]->getName(), QVariant::fromValue(functions[id]));
    }
}

void IntegralFunctionDialog::setFunction(const IntegralFunctionPtr &f)
{
    mEdit = true;
    qDebug() << "IntegralFunctionDialog::setFunction("<<f->getName()<<")";
    //if(mFunction != 0) delete mFunction;
    mFunction = f;
    ui->integralName->setText(f->getName());
    ui->integralIntegrationVariable->setText(f->getIntegrationVariable());

    int index = ui->integralRange->findData(f->getRange());
    if(index != -1)
        ui->integralRange->setCurrentIndex(index);

    // Set DZ
    QString dzName = f->getTransmissionDZName();
    index = ui->DZFunction->findData(dzName, Qt::DisplayRole);
    if(index != -1) {
        ui->DZFunction->setCurrentIndex(index);
    }

    setWindowTitle(tr("Edit IntegralFunction ") + f->getName());
    if(f != 0) {
        QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->integralView->model());
        model->clear();
        foreach(FunctionPtr af, mFunction->getFunctions()) {
            addFunctionItem(af);
        }
    }
}

// ============================= PRIVATE =======================
void IntegralFunctionDialog::addFunction(const FunctionPtr &f)
{
    if(f != 0) {
        mFunction->setParameters(f, mFunction->getIntegrationVariable());
        addFunctionItem(f);
        mFunction->addFunction(f);

        emit expressionChanged();
    } else {
        //XXX
        qDebug() << "IntegralFunctionDialog::addFunctionItem: ERROR INVALID FUNCTION TYPE";
    }

}

void IntegralFunctionDialog::addFunctionItem(const FunctionPtr &f)
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

void IntegralFunctionDialog::setFunctionConfiguration(const FunctionPtr &f)
{
    qDebug() << "Set function configuration for "<< f->getName();
    QString parameter = ui->functionParameters->text();
    qDebug() << "with parameters: "<<parameter;
    if(parameter.isEmpty())
        mFunction->setParameters(f, ui->integralIntegrationVariable->text());
    else
        mFunction->setParameters(f, parameter);
}

void IntegralFunctionDialog::useFunction(const FunctionPtr &f)
{
    if(f != 0) {
        mFunctionInUse = f;
        ui->functionConfigWidget->show();
        ui->functionNoSelectionWidget->hide();
        ui->functionParameters->setText(mFunction->getParameters(f));
        ui->functionExpression->setText("<b>"+f->getName()+ "</b>(" + f->getVariable() + ") = " + f->getExpression());
    } else {
        //XXX:
        qDebug() << "IntegralFunctionDialog::useFunction: ERROR, INVALID FUNCTION TYPE";
    }
}

// ============================= SLOTS ==========================

void IntegralFunctionDialog::addFunction()
{
    FunctionSelectionDialog dialog(this, AbstractFunction::Function | AbstractFunction::HierarchicalFunction);
    if(dialog.exec() == QDialog::Accepted) {
        // Don't copy for integral, it has to be updated to the parent curve
        // XXX: link any change to parent to integral
        FunctionPtr function = dialog.getSelectedFunction();
        if(function != 0) {
            addFunction(dialog.getSelectedFunction());
        } else {
            qDebug() << "IntegralFunctionDialog::addFunction  -- XXX: fatal error: invalid function type!";
        }
    }
}

void IntegralFunctionDialog::removeFunction()
{
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->integralView->model());
    QModelIndex index = ui->integralView->currentIndex();
    FunctionPtr f = index.data(Qt::UserRole).value<FunctionPtr>();
    // Detach function, don't delete it (linked to other function used elsewhere)!
    mFunction->removeFunction(f);
    model->removeRow(index.row());
    emit expressionChanged();
}


void IntegralFunctionDialog::accept()
{
    qDebug() << "IntegralFunctionDialog::accept()";
    bool mayClose = false;
    if(!mEdit && Singleton<FunctionsSingleton>::Instance().functionNameExists(ui->integralName->text())) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Name Conflict"), tr("Another curve with the name ") + ui->integralName->text() + tr(" already exists. Do you want to modify the name?"), QMessageBox::Yes|QMessageBox::No);
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
        mFunction->setName(ui->integralName->text());
        mFunction->setRange(static_cast<IntegralFunction::Range>(ui->integralRange->itemData(ui->integralRange->currentIndex()).toUInt()));
        QString transmissionDZ = ui->DZFunction->itemData(ui->DZFunction->currentIndex(), Qt::DisplayRole).toString();
        mFunction->setTransmissionDZ(transmissionDZ);
        qDebug() << "transmissionDZ " << transmissionDZ;

        // Save current function parameters
        QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->integralView->model());
        if(model != 0) {
            QModelIndex index = ui->integralView->currentIndex();
            QVariant item = model->data(index, Qt::UserRole);
            if(item.isValid()) {
                FunctionPtr f = item.value<FunctionPtr>();
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

        mFunction->updateLinkedCurve(true);

        QDialog::accept();
    }
}

void IntegralFunctionDialog::updateExpression()
{
    ui->integralExpression->setText(mFunction->getIntegralExpression());
}

void IntegralFunctionDialog::functionSelectionChanged( const QModelIndex & newSelection, const QModelIndex & previousSelection)
{
    qDebug() << "################# functionSelectionChanged";
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui->integralView->model());
    if(model != 0) {
            QVariant item = model->data(previousSelection, Qt::UserRole);
            if(item.isValid()) {
                FunctionPtr f = item.value<FunctionPtr>();
                if(f != 0) {
                    qDebug() << "Previous: " << f->getName();
                    setFunctionConfiguration(f);
                }
            }

            item = model->data(newSelection, Qt::UserRole);
            if(item.isValid()) {
                FunctionPtr f = item.value<FunctionPtr>();
                if(f != 0) {
                    qDebug() << "New: " << f->getName();
                    useFunction(f);
                }
            }
    }
}

void IntegralFunctionDialog::parametersEdited()
{
    if(mFunctionInUse != 0) {
        setFunctionConfiguration(mFunctionInUse);
        updateExpression();
    }
}

void IntegralFunctionDialog::integrationVariableEdited()
{
    mFunction->setIntegrationVariable(ui->integralIntegrationVariable->text());
    updateExpression();
}

void IntegralFunctionDialog::configureTransmission()
{
    qDebug() << "IntegralFunctionDialog::configureTransmission()";
    TransmissionDialog dialog(this);
    dialog.exec();
}
