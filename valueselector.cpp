#include "valueselector.h"
#include "ui_valueselector.h"
#include "projectsingleton.h"
#include "function.h"
#include "variablefactory.h"

#include "slider.h"
#include "sliderconfigdialog.h"

#include <QDebug>
#include <QSettings>

ValueSelector::ValueSelector(const QString &name, const FunctionPtr &f, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ValueSelector)
{
    ui->setupUi(this);
    mFunction = f;

    //minValueChanged(ui->variableMin->value());
    //maxValueChanged(ui->variableMax->value());

    mName = name;

    mVariable = mFunction->getVariableFactory()->getVariableAddress(name);
    *mVariable = ui->variableValue->value();

    ui->variableName->setText(name + " = ");

    // XXX: use global settings
    //double minSliderValue = Singleton<GlobalSettingsSingleton>::Instance().getSliderMin();
    //double maxSliderValue = Singleton<GlobalSettingsSingleton>::Instance().getSliderMax();
    double minSliderValue = 0;
    double maxSliderValue = 100;
    setRange(minSliderValue, maxSliderValue, 0);

    //connect(ui->variableValue, SIGNAL(valueChanged(double)), this, SLOT(variableValueChanged(double)));
    //connect(ui->variableSlider, SIGNAL(valueChanged(double)), this, SLOT(sliderValueChanged(double)));
    connect(ui->variableValue, SIGNAL(editingFinished()), this, SLOT(valueEntered()));
    connect(ui->variableSlider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(ui->variableSlider, SIGNAL(rightClicked(const Slider *)), this, SLOT(configureSlider(const Slider *)));
}

ValueSelector::~ValueSelector()
{
    delete ui;
}

void ValueSelector::save()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(mName);
    settings->setValue("name", mName);
    settings->setValue("value", ui->variableValue->value());
    settings->setValue("min", ui->variableSlider->lowerBound());
    settings->setValue("max", ui->variableSlider->upperBound());
    settings->setValue("step", ui->variableSlider->scaleStepSize());
    settings->endGroup();
}

void ValueSelector::loadFromConfig()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Variables/"+QString::number(mFunction->getId())+"/"+mName);
    qDebug() << "ValueSelector::loadFromConfig() - loading from " << settings->group();
    setRange(settings->value("min", 0).toDouble(), settings->value("max", 100).toDouble(), settings->value("step", 0).toDouble());
    ui->variableValue->setValue(settings->value("value", 0).toDouble());
    ui->variableSlider->setValue(ui->variableValue->value());
    settings->endGroup();
}

// ===================== SLOTS ===========================
void ValueSelector::variableValueChanged(double val)
{
    qDebug() << "value changed";
    *mVariable = val;

    emit valueChanged(val);
    emit valueChanged(mName, val);
}

void ValueSelector::sliderReleased()
{
    qDebug() << "Slider released slot";
    double val = ui->variableSlider->value();
    ui->variableValue->setValue(val);
    variableValueChanged(val);
}

void ValueSelector::valueEntered()
{
    double val = ui->variableValue->value();
    ui->variableSlider->setValue(val);
    variableValueChanged(val);
}

void ValueSelector::sliderValueChanged(double val)
{
    ui->variableValue->setValue(val);
    *mVariable = val;
}

void ValueSelector::setRange(double min, double max, double step)
{
    qDebug() << "ValueSelector::setRange("<<min<<", "<<max<<")";
    double value = ui->variableValue->value();
    ui->variableSlider->setScaleStepSize(step);
    ui->variableSlider->setLowerBound(min, value);
    ui->variableSlider->setUpperBound(max, value);
}

void ValueSelector::configureSlider(const Slider *s)
{
    SliderConfigDialog dialog(this);
    dialog.initFromSlider(s);
    if(dialog.exec() == QDialog::Accepted) {
        qDebug() << "dialog accepted";
        if(!dialog.applyToAll()) {
            setRange(dialog.getMin(), dialog.getMax(), dialog.getStep());
        } else {
            emit configureAllSliders(dialog.getMin(), dialog.getMax(), dialog.getStep());
        }
    }

}
