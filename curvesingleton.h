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
    void removeCurveFromId(int id);
    void removeCurve(Curve *curve);
    QMap<int, Curve *> getCurves() const;
    Curve* getCurve(int id) const;

    void save();
    void loadFromSettings();
    void clear();

private:
    QMap<int, Curve *> mCurves;

signals:
    void newCurveAvailable(Curve *);
};

#endif // CURVESINGLETON_H
