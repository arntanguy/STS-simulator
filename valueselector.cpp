#include "valueselector.h"
#include "ui_valueselector.h"
#include "projectsingleton.h"
#include "function.h"
#include "variablefactory.h"

#include <qwt_slider.h>
#include <QDebug>
#include <QSettings>

ValueSelector::ValueSelector(const QString &name, Function *f, QWidget *parent) :
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

    //double minSliderValue = Singleton<GlobalSettingsSingleton>::Instance().getSliderMin();
    //double maxSliderValue = Singleton<GlobalSettingsSingleton>::Instance().getSliderMax();
    double minSliderValue = 0;
    double maxSliderValue = 100;
    sliderRangeChanged(minSliderValue, maxSliderValue);

    connect(ui->variableValue, SIGNAL(valueChanged(double)), this, SLOT(variableValueChanged(double)));
    connect(ui->variableSlider, SIGNAL(valueChanged(double)), this, SLOT(sliderValueChanged(double)));
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
    settings->endGroup();
}

void ValueSelector::loadFromConfig()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Variables/"+QString::number(mFunction->getId())+"/"+mName);
    qDebug() << "ValueSelector::loadFromConfig() - loading from " << settings->group();
    ui->variableValue->setValue(settings->value("value", 0).toDouble());
    //double minSliderValue = Singleton<GlobalSettingsSingleton>::Instance().getSliderMin();
    //double maxSliderValue = Singleton<GlobalSettingsSingleton>::Instance().getSliderMax();
    //sliderRangeChanged(minSliderValue, maxSliderValue);
    settings->endGroup();
}

// ===================== SLOTS ===========================
void ValueSelector::variableValueChanged(double val)
{
    qDebug() << "value changed";
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

void ValueSelector::sliderRangeChanged(double min, double max)
{
    ui->variableSlider->setValue(min);
    ui->variableSlider->setValue(max);
}
