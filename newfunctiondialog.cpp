#include "newfunctiondialog.h"
#include "ui_newfunctiondialog.h"

NewFunctionDialog::NewFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFunctionDialog)
{
    ui->setupUi(this);
}

NewFunctionDialog::~NewFunctionDialog()
{
    delete ui;
}
