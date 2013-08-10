#include "functionssingleton.h"
#include "projectsingleton.h"
#include "function.h"
#include "hierarchicalfunction.h"
#include "integralfunction.h"
#include "abstractfunction.h"
#include "differentialfunction.h"
#include "curve.h"
#include "functioncurve.h"

#include <QStringList>
#include <QDebug>

FunctionsSingleton::FunctionsSingleton()
{
}
FunctionsSingleton::~FunctionsSingleton()
{
    clear();
}

void FunctionsSingleton::clear()
{
    qDebug() << "FunctionsSingleton::clear()";
    //qDeleteAll(mFunctions);
    mFunctions.clear();
    AbstractFunction::resetStaticId();
}

void FunctionsSingleton::addFunction(const AbstractFunctionPtr &f)
{
    qDebug() << "FunctionSingleton::addFunction: adding function "<<f->getName();
    if(f != 0) {
        QSharedPointer<AbstractFunction> pf(f);
        mFunctions[f->getId()] = pf;
    } else {
        qDebug() << "FunctionSingleton::addFunction: CANNOT ADD A NULL FUNCTION";
    }
}

void FunctionsSingleton::removeFunction(const AbstractFunctionPtr &f)
{
    qDebug() << "FunctionsSingleton::removeFunction";
    qDebug() << "deleting curve " << f->getName();
    if(f != 0) {
        Curve *c = f->getCurve();
        if(c != 0) {
            c->detachFromAll();
        }
        QMap<int, QSharedPointer<AbstractFunction>>::iterator it = mFunctions.find(f->getId());
        if(it != mFunctions.end()) {
            if(it.value() != 0) {
                qDebug() << "FunctionsSingleton::removeFunction() - deleting " << it.value()->getName();
                mFunctions.erase(it);
            }
        }
    }
}

AbstractFunctionPtr FunctionsSingleton::getFunctionById(int id)
{
    foreach(QSharedPointer<AbstractFunction> f, mFunctions) {
        if(f->getId() == id) {
            return f;
        }
    }
    return AbstractFunctionPtr(0);
}

QList<int> FunctionsSingleton::getFunctionIds() const
{
    return mFunctions.keys();
}

bool FunctionsSingleton::functionNameExists(const QString& name) const
{
    foreach(AbstractFunctionPtr f, mFunctions) {
        if(f != 0) {
            if(f->getName() == name) {
                return true;
            }
        }
    }
    return false;
}

void FunctionsSingleton::loadFromSettings()
{
    qDebug() << "FunctionsSingleton::loadFromSettings()";
	QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();

    settings->beginGroup("Functions/Function/");
    QStringList groups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, groups) {
        FunctionPtr f(new Function(functionId.toInt()));
        f->loadFromConfig("Functions/Function/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/HierarchicalFunction/");
    QStringList hGroups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, hGroups) {
        HierarchicalFunctionPtr f(new HierarchicalFunction(functionId.toInt()));
        f->loadFromConfig("Functions/HierarchicalFunction/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/IntegralFunction/");
    QStringList iGroups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, iGroups) {
        IntegralFunctionPtr f(new IntegralFunction(functionId.toInt()));
        f->loadFromConfig("Functions/IntegralFunction/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/DifferentialFunction/");
    QStringList dGroups = settings->childGroups();
    settings->endGroup();
    foreach(QString functionId, dGroups) {
        DifferentialFunctionPtr f(new DifferentialFunction(functionId.toInt()));
        f->loadFromConfig("Functions/DifferentialFunction/"+functionId);
        addFunction(f);
    }

    foreach(QSharedPointer<AbstractFunction> f, mFunctions) {
        f->updateLinkedCurve(true);
    }
}

void FunctionsSingleton::save()
{
    qDebug() << "FunctionsSingleton::save() : Saving all functions...";
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();

    // First cleanup the curves
    settings->beginGroup("Functions");
    settings->remove("");
    settings->endGroup();


    // Now save all the curves
    foreach(int fId, mFunctions.keys()) {
        qDebug() << "Function " << fId;
        mFunctions[fId]->save("Functions");
    }
}
