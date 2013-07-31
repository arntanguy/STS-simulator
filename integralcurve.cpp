#include "integralcurve.h"
#include "plotwidget.h"
#include "integralfunction.h"
#include "projectsingleton.h"

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
    setStepNumber(2);
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
    //update();
}

int IntegralCurve::getStepNumber() const
{
    return mStepNumber;
}

// ========================= PUBLIC VIRTUAL ==============================
void IntegralCurve::update()
{
    FunctionCurve::update();
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
    update();
}
