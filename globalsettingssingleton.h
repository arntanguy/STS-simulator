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
    void setResolution(double resolution);
    void setIntegralResolution(double resolution);

    void setOverlayOpacity(float opacity);

    double getMin() const {
        return mMin;
    }
    double getMax() const {
        return mMax;
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

private:
    void init();

private:
    bool mUpdateCurve;
    bool mUpdateIntegral;

    double mMin;
    double mMax;
    double mResolution;
    double mIntegralResolution;
    float mOverlayOpacity;
};

#endif // GLOBALSETTINGSSINGLETON_H
