#include "integralcurve.h"
#include "plotwidget.h"
#include "integralfunction.h"
#include "projectsingleton.h"
#include "globalsettingssingleton.h"

IntegralCurve::IntegralCurve() : FunctionCurve()
{
    init();
}

IntegralCurve::IntegralCurve(unsigned int id) : FunctionCurve(id)
{
    init();
}

void IntegralCurve::init()
{
    mType = Curve::Integral;
    initGlobalSettings();

    GlobalSettingsSingleton *singleton = &Singleton<GlobalSettingsSingleton>::Instance();
    connect(singleton, SIGNAL(integralSettingsUpdated()), this, SLOT(initGlobalSettings()));
}

// XXX : check if updated correctly
void IntegralCurve::initGlobalSettings()
{
    FunctionCurve::initGlobalSettings();
    GlobalSettingsSingleton *singleton = &Singleton<GlobalSettingsSingleton>::Instance();
    setStepNumber(singleton->getIntegralResolution());
}

// ========================= PUBLIC =====================================
void IntegralCurve::setFunction(IntegralFunction *f)
{
    FunctionCurve::setFunction(f);
}

void IntegralCurve::setStepNumber(int stepNumber)
{
    if(mStepNumber != stepNumber) {
        mStepNumber = stepNumber;
        mNeedsUpdate = true;
    }
}

int IntegralCurve::getStepNumber() const
{
    return mStepNumber;
}

// ========================= PUBLIC VIRTUAL ==============================
void IntegralCurve::update(bool forceUpdate)
{
    FunctionCurve::update(forceUpdate);
}

void IntegralCurve::updateData()
{
    qDebug() << "IntegralCurve::updateData()";
    IntegralFunction *f = dynamic_cast<IntegralFunction*>(mFunction);
    if(f != 0) {
        qDebug() << "IntegralCurve::updateData() -- valid integral function";
        // XXX: set stepNumber based on curve preferences
        IntegralData data = f->integrate(getMin(), getMax(), getResolution(), mStepNumber);
        mXData = data.x;
        mYData = data.y;
    }
    mNeedsUpdate = false;
}

void IntegralCurve::save()
{
    FunctionCurve::save();

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(getId()));
    settings->setValue("integralResolution", getStepNumber());
    settings->endGroup();
}

void IntegralCurve::loadFromSettings()
{
    FunctionCurve::loadFromSettings();

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(getId()));
    setStepNumber(settings->value("integralResolution", 10).toInt());
    settings->endGroup();
    update(true);
}
