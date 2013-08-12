#include "globalsettingssingleton.h"
#include "projectsingleton.h"

#include <QSettings>
#include <QDebug>

GlobalSettingsSingleton::GlobalSettingsSingleton(QObject *parent) : QObject(parent)
{
    init();
}

void GlobalSettingsSingleton::init()
{
    mUpdateCurve = false;
    mUpdateIntegral = false;
    mMin = 0;
    mMax = 1;
    mResolution = 100;
}

void GlobalSettingsSingleton::loadFromSettings()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("GlobalSettings");
    settings->beginGroup("Plot");
    setMin(settings->value("xmin", 0).toDouble());
    setMax(settings->value("xmax", 1).toDouble());
    setResolution(settings->value("resolution", 1000).toInt());
    setIntegralResolution(settings->value("integralResolution", 2).toInt());
    settings->endGroup();

    settings->beginGroup("Overlay");
    setOverlayOpacity(settings->value("opacity", 0.8).toDouble());
    settings->endGroup();
    settings->endGroup();

    update();
}

void GlobalSettingsSingleton::save()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("GlobalSettings");
    settings->remove("");
    settings->beginGroup("Plot");
    settings->setValue("xmin", mMin);
    settings->setValue("xmax", mMax);
    settings->setValue("resolution", mResolution);
    settings->setValue("integralResolution", mIntegralResolution);
    settings->endGroup();

    settings->beginGroup("Overlay");
    settings->setValue("opacity", mOverlayOpacity);
    settings->endGroup();
    settings->endGroup();
}

void GlobalSettingsSingleton::setMin(double min)
{
    if(mMin != min) {
        mUpdateCurve = true;
        mMin = min;
    }
}

void GlobalSettingsSingleton::setMax(double max)
{
    if(mMax != max) {
        mUpdateCurve = true;
        mMax = max;
    }
}

void GlobalSettingsSingleton::setResolution(double resolution)
{
    if(resolution != mResolution) {
        mUpdateCurve = true;
        mResolution = resolution;
    }
}

void GlobalSettingsSingleton::setIntegralResolution(double resolution)
{
    if(mIntegralResolution != resolution) {
        mIntegralResolution = resolution;
        mUpdateIntegral = true;
    }
}


void GlobalSettingsSingleton::setOverlayOpacity(float opacity)
{
    mOverlayOpacity = opacity;
}

/**
 * \brief Emits signal settingsUpdated if needed
 **/
void GlobalSettingsSingleton::update()
{
    if(mUpdateCurve)
        emit curveSettingsUpdated();
    if(mUpdateIntegral)
        emit integralSettingsUpdated();

    emit overlayOpacityUpdated();

    mUpdateCurve = false;
    mUpdateIntegral = false;
}
