#include "functionselectiondialog.h"
#include "ui_functionselectiondialog.h"

FunctionSelectionDialog::FunctionSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FunctionSelectionDialog)
{
    ui->setupUi(this);
}

FunctionSelectionDialog::~FunctionSelectionDialog()
{
    delete ui;
}
