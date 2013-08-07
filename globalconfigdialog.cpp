#include "globalconfigdialog.h"
#include "ui_globalconfigdialog.h"

#include "globalsettingssingleton.h"

#include <QPushButton>

GlobalConfigDialog::GlobalConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlobalConfigDialog)
{
    ui->setupUi(this);

    init();

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
    ui->plotYMin->setValue(mSingleton->getYMin());
    ui->plotYMax->setValue(mSingleton->getYMax());
    ui->plotYAuto->setChecked(mSingleton->isAutoY());
    ui->plotResolution->setValue(mSingleton->getResolution());
    ui->integralResolution->setValue(mSingleton->getIntegralResolution());

    ui->overlayOpacity->setValue(mSingleton->getOverlayOpacity());

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
    connect(ui->plotYAuto, SIGNAL(toggled(bool)), this, SLOT(autoYChecked(bool)));
}

// =============================== SLOTS ===============================================
void GlobalConfigDialog::autoYChecked(bool checked)
{
    ui->plotYAuto->setChecked(checked);
    ui->plotYMin->setEnabled(!checked);
    ui->plotYMax->setEnabled(!checked);
}

void GlobalConfigDialog::apply()
{
    mSingleton->setMin(ui->plotMin->value());
    mSingleton->setMax(ui->plotMax->value());
    mSingleton->setYMin(ui->plotYMin->value());
    mSingleton->setYMax(ui->plotYMax->value());
    mSingleton->setAutoY(ui->plotYAuto->isChecked());
    mSingleton->setResolution(ui->plotResolution->value());
    mSingleton->setIntegralResolution(ui->integralResolution->value());
    mSingleton->setOverlayOpacity(ui->overlayOpacity->value());
    mSingleton->update();
}

void GlobalConfigDialog::accept()
{
    apply();
    QDialog::accept();
}

