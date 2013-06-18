#include "curvesingleton.h"
#include "projectsingleton.h"
#include "curve.h"

CurveSingleton::CurveSingleton()
{
}

void CurveSingleton::addCurve(Curve *curve)
{
    mCurves[curve->getId()] = curve;
    emit newCurveAvailable(curve);
}

void CurveSingleton::removeCurve(Curve *curve)
{
    mCurves.remove(curve->getId());
}

QMap<unsigned int, Curve *> CurveSingleton::getCurves() const
{
    return mCurves;
}

Curve* CurveSingleton::getCurve(unsigned int id) const
{
    return mCurves[id];
}

void CurveSingleton::loadFromSettings()
{
    qDebug()<< "CurveSingleton::loadFromSettings()";
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    // First cleanup the curves
    settings->beginGroup("Curves");
    QStringList groups = settings->childGroups();
    settings->endGroup();

    foreach(QString curveId, groups) {
        qDebug() << "Current group " << settings->group();
        unsigned int id = settings->value("Curves/"+curveId+"/id", -1).toUInt();
        Curve *curve = new Curve(id);
        curve->loadFromSettings();
        addCurve(curve);
    }
}

void CurveSingleton::save()
{
    qDebug() << "Saving all curves...";
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();

    // First cleanup the curves
    settings->beginGroup("Curves");
    settings->remove("");
    settings->endGroup();


    // Now save all the curves
    QMapIterator<unsigned int, Curve*> i(mCurves);
    while (i.hasNext()) {
        i.next();
        i.value()->save();
    }
}
