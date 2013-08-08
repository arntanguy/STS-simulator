#include "curvesingleton.h"
#include "projectsingleton.h"
#include "curve.h"
#include "functioncurve.h"
#include "variablefactory.h"

#include "function.h"
#include "integralfunction.h"
#include "integralcurve.h"
#include "functionssingleton.h"
#include "differentialcurve.h"

CurveSingleton::CurveSingleton()
{
}

void CurveSingleton::addCurve(Curve *curve)
{
    qDebug() << "CurveSingleton::addCurve - Adding curve "<<curve->title().text();
    mCurves[curve->getId()] = curve;
    emit newCurveAvailable(curve);
}

void CurveSingleton::removeCurveFromId(int id)
{
    if(mCurves.contains(id))
        removeCurve(mCurves[id]);
}
void CurveSingleton::removeCurve(Curve *curve)
{
    curve->detachFromAll();
    mCurves.remove(curve->getId());
    delete curve;
}

void CurveSingleton::clear()
{
    qDebug() << "CurveSingleton::clear()";
    foreach(int id, mCurves.keys()) {
        qDebug() << "Curve id "<< id;
        removeCurveFromId(id);
    }
    Curve::resetStaticId();
}

QMap<int, Curve *> CurveSingleton::getCurves() const
{
    return mCurves;
}

Curve* CurveSingleton::getCurve(int id) const
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
        int id = settings->value("Curves/"+curveId+"/id", -1).toUInt();
        Curve::Type type= static_cast<Curve::Type>(settings->value("Curves/"+curveId+"/type", Curve::Experimental).toUInt());
        if(type == Curve::Experimental) {
            Curve *curve = new Curve(id);
            qDebug() << "Loading experimental curve " << curve->getId();
            curve->loadFromSettings();
            addCurve(curve);
        } else if(type == Curve::Function){
            FunctionCurve *curve = new FunctionCurve(id);
            qDebug() << "Loading Function curve " << curve->getId();
            curve->loadFromSettings();
            addCurve(curve);
        } else if(type == Curve::Integral) {
            IntegralCurve *curve = new IntegralCurve(id);
            qDebug() << "Loading Integral curve " << curve->getId();
            curve->loadFromSettings();
            addCurve(curve);
        } else {
            DifferentialCurve *curve = new DifferentialCurve(id);
            qDebug() << "Loading Differential curve " << curve->getId();
            curve->loadFromSettings();
            addCurve(curve);
        }
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
    QMapIterator<int, Curve*> i(mCurves);
    while (i.hasNext()) {
        i.next();
        i.value()->save();
    }
}
