#include "valueselector.h"
#include "ui_valueselector.h"

#include <qwt_slider.h>
#include <QDebug>

ValueSelector::ValueSelector(const QString &name, double *variable, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ValueSelector)
{
    ui->setupUi(this);

    minValueChanged(ui->variableMin->value());
    maxValueChanged(ui->variableMax->value());

    mName = name;

    mVariable = variable;
    *mVariable = ui->variableValue->value();

    ui->variableName->setText(name + " = ");


    connect(ui->variableMin, SIGNAL(valueChanged(double)), this, SLOT(minValueChanged(double)));
    connect(ui->variableMax, SIGNAL(valueChanged(double)), this, SLOT(maxValueChanged(double)));
    connect(ui->variableValue, SIGNAL(valueChanged(double)), this, SLOT(variableValueChanged(double)));
    connect(ui->variableSlider, SIGNAL(valueChanged(double)), this, SLOT(sliderValueChanged(double)));
}

ValueSelector::~ValueSelector()
{
    delete ui;
}

// ===================== SLOTS ===========================
void ValueSelector::minValueChanged(double min)
{
    ui->variableSlider->setScale(min, ui->variableMax->value());
    ui->variableValue->setMinimum(min);
    if(ui->variableValue->value() < min) {
        ui->variableValue->setValue(min);
        ui->variableSlider->setValue(min);
    }
}
void ValueSelector::maxValueChanged(double max)
{
    ui->variableSlider->setScale(ui->variableMin->value(), max);
    ui->variableValue->setMaximum(max);
    if(ui->variableValue->value() > max) {
        ui->variableValue->setValue(max);
        ui->variableSlider->setValue(max);
    }
}
void ValueSelector::variableValueChanged(double val)
{
    qDebug() << "value changed";
    if(val > ui->variableMax->value()) {
        ui->variableMax->setValue(val);
    } else if(val < ui->variableMin->value()) {
        ui->variableMin->setValue(val);
    }
    ui->variableSlider->setValue(val);
    *mVariable = val;

    emit valueChanged(val);
    emit valueChanged(mName, val);
}

void ValueSelector::sliderValueChanged(double val)
{
    ui->variableValue->setValue(val);
    *mVariable = val;
}
