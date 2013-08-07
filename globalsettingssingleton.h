#ifndef GLOBALSETTINGSSINGLETON_H
#define GLOBALSETTINGSSINGLETON_H

#include <QObject>
#include "singleton.h"

class GlobalSettingsSingleton : public QObject
{
    Q_OBJECT
public:
    GlobalSettingsSingleton(QObject *parent = 0);

    void setMin(double min);
    void setMax(double max);
    void setYMin(double min);
    void setYMax(double max);
    void setAutoY(bool state);
    void setResolution(double resolution);
    void setIntegralResolution(double resolution);

    void setOverlayOpacity(float opacity);

    double getMin() const {
        return mMin;
    }
    double getMax() const {
        return mMax;
    }
    double getYMin() const {
        return mYMin;
    }
    double getYMax() const {
        return mYMax;
    }
    bool isAutoY() const {
        return mYAuto;
    }

    double getResolution() const {
        return mResolution;
    }
    double getIntegralResolution() const {
        return mIntegralResolution;
    }
    float getOverlayOpacity() const {
        return mOverlayOpacity;
    }

    void loadFromSettings();
    void save();
    void update();

Q_SIGNALS:
    void curveSettingsUpdated();
    void integralSettingsUpdated();
    void overlayOpacityUpdated();
    void plotAutoYChanged(bool);

private:
    void init();

private:
    bool mUpdateCurve;
    bool mUpdateIntegral;
    bool mAutoYRangeUpdated;

    double mMin, mMax;
    double mYMin, mYMax;
    bool mYAuto;
    double mResolution, mIntegralResolution;
    float mOverlayOpacity;
};

#endif // GLOBALSETTINGSSINGLETON_H
