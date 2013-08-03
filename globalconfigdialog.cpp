#include "globalconfigdialog.h"
#include "ui_globalconfigdialog.h"

#include "globalsettingssingleton.h"

GlobalConfigDialog::GlobalConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalConfigDialog)
{
    ui->setupUi(this);

    init();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

GlobalConfigDialog::~GlobalConfigDialog()
{
    delete ui;
}

void GlobalConfigDialog::init()
{
    mSingleton = &Singleton<GlobalSettingsSingleton>::Instance();
    ui->plotMin->setValue(mSingleton->getMin());
    ui->plotMax->setValue(mSingleton->getMax());
    ui->plotResolution->setValue(mSingleton->getResolution());
    ui->integralResolution->setValue(mSingleton->getIntegralResolution());

    ui->overlayOpacity->setValue(mSingleton->getOverlayOpacity());
}

void GlobalConfigDialog::accept()
{
    mSingleton->setMin(ui->plotMin->value());
    mSingleton->setMax(ui->plotMax->value());
    mSingleton->setResolution(ui->plotResolution->value());
    mSingleton->setIntegralResolution(ui->integralResolution->value());
    mSingleton->setOverlayOpacity(ui->overlayOpacity->value());
    mSingleton->update();
    QDialog::accept();
}
