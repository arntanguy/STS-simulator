#ifndef CURVESINGLETON_H
#define CURVESINGLETON_H

#include <QMap>
#include <QObject>

#include "singleton.h"

class Curve;

class CurveSingleton : public QObject
{
    Q_OBJECT
public:
    CurveSingleton();
    void addCurve(Curve *curve);
    void removeCurve(Curve *curve);
    QMap<unsigned int, Curve *> getCurves() const;
    Curve* getCurve(unsigned int id) const;

    void save();
    void loadFromSettings();

private:
    QMap<unsigned int, Curve *> mCurves;

signals:
    void newCurveAvailable(Curve *);
};

#endif // CURVESINGLETON_H
