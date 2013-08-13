#include "sliderconfigdialog.h"
#include "ui_sliderconfigdialog.h"

#include "slider.h"

SliderConfigDialog::SliderConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SliderConfigDialog)
{
    ui->setupUi(this);
}

SliderConfigDialog::~SliderConfigDialog()
{
    delete ui;
}

void SliderConfigDialog::initFromSlider(const Slider *s)
{
    ui->min->setValue(s->minimum());
    ui->max->setValue(s->maximum());
}

double SliderConfigDialog::getMin() const
{
    return ui->min->value();
}

double SliderConfigDialog::getMax() const
{
    return ui->max->value();
}

double SliderConfigDialog::getStep() const
{
    return ui->step->value();
}

bool SliderConfigDialog::applyToAll() const
{
    return (ui->applyToAll->checkState() == Qt::Checked);
}
