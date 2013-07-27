#include "curvesingleton.h"
#include "projectsingleton.h"
#include "curve.h"
#include "functioncurve.h"

#include "integralfunction.h"
#include "integralcurve.h"
#include "functionssingleton.h"

CurveSingleton::CurveSingleton()
{
}

void CurveSingleton::addCurve(Curve *curve)
{
    qDebug() << "CurveSingleton::addCurve - Adding curve "<<curve->title().text();
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
        } else {
            IntegralCurve *curve = new IntegralCurve(id);
            qDebug() << "Loading Integral curve " << curve->getId();
            curve->loadFromSettings();
            addCurve(curve);
        }
    }

    //XXX: remove, just test
    // ==========================================================
    IntegralFunction *f = new IntegralFunction();
    f->setName("IntegralFunction");
    f->setExpression("e");
    Singleton<FunctionsSingleton>::Instance().addFunction(f);
    IntegralCurve *curve = new IntegralCurve();
    curve->setTitle("Integral");
    curve->setFunction(f);
    addCurve(curve);
    f->setCurve(curve);
    // ============================================================

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
