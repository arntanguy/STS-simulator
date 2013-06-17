#include "curvesingleton.h"
#include "curve.h"

CurveSingleton::CurveSingleton()
{
}

void CurveSingleton::addCurve(Curve *curve)
{
    mCurves[curve->title().text()] = curve;
    emit newCurveAvailable(curve);
}

void CurveSingleton::removeCurve(Curve *curve)
{
    mCurves.remove(curve->title().text());
}

QMap<QString, Curve *> CurveSingleton::getCurves() const
{
    return mCurves;
}
