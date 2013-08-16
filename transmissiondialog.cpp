#include "transmissiondialog.h"
#include "ui_transmissiondialog.h"
#include "functionssingleton.h"
#include "singleton.h"
#include "transmissionfunction.h"

TransmissionDialog::TransmissionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransmissionDialog)
{
    ui->setupUi(this);

    init();

    //connect(this, SIGNAL(accepted()), this, SLOT(accept()));
}

void TransmissionDialog::init()
{
    mTransmission = Singleton<FunctionsSingleton>::Instance().getTransmissionFunction();

    ui->phi->setValue(mTransmission->getPhi());
    ui->Z0->setValue(mTransmission->getZ0());
}

TransmissionDialog::~TransmissionDialog()
{
    delete ui;
}

void TransmissionDialog::accept()
{
    if(mTransmission == 0) {
        qDebug() << "ERROR: NO TRANSMISSION!!";
        QDialog::reject();
    } else {
        qDebug () << "Set Phi: " <<ui->phi->value();
        mTransmission->setPhi(ui->phi->value());
        mTransmission->setZ0(ui->Z0->value());
    }

    QDialog::accept();
}
