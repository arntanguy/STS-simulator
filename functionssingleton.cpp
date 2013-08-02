#include "functionssingleton.h"
#include "projectsingleton.h"
#include "function.h"
#include "hierarchicalfunction.h"
#include "integralfunction.h"
#include "abstractfunction.h"
#include "curve.h"
#include "functioncurve.h"

#include <QStringList>
#include <QDebug>

FunctionsSingleton::FunctionsSingleton()
{
}
FunctionsSingleton::~FunctionsSingleton()
{
}

void FunctionsSingleton::addFunction(AbstractFunction *f)
{
    qDebug() << "FunctionSingleton::addFunction: adding function "<<f->getName();
    if(f != 0) {
        mFunctions[f->getId()] = f;
    } else {
        qDebug() << "FunctionSingleton::addFunction: CANNOT ADD A NULL FUNCTION";
    }
}

void FunctionsSingleton::removeFunction(AbstractFunction *f)
{
    if(f != 0) {
        Curve *c = f->getCurve();
        if(c != 0) {
            c->detachFromAll();
        }
        QMap<int, AbstractFunction *>::iterator it = mFunctions.find(f->getId());
        if(it != mFunctions.end()) {
            qDebug() << "FunctionsSingleton::removeFunction() - deleting " << it.value()->getName();
            mFunctions.erase(it);
            delete f;
        }
    }
}

AbstractFunction *FunctionsSingleton::getFunctionById(int id)
{
    foreach(AbstractFunction *f, mFunctions) {
        if(f->getId() == id) {
            return f;
        }
    }
    return 0;
}

QList<int> FunctionsSingleton::getFunctionIds() const
{
    return mFunctions.keys();
}


void FunctionsSingleton::loadFromSettings()
{
    qDebug() << "FunctionsSingleton::loadFromSettings()";
	QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();

    settings->beginGroup("Functions/Function/");
    QStringList groups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, groups) {
        Function *f = new Function(functionId.toInt());
        f->loadFromConfig("Functions/Function/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/HierarchicalFunction/");
    QStringList hGroups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, hGroups) {
        HierarchicalFunction *f = new HierarchicalFunction(functionId.toInt());
        f->loadFromConfig("Functions/HierarchicalFunction/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/IntegralFunction/");
    QStringList iGroups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, iGroups) {
        IntegralFunction *f = new IntegralFunction(functionId.toInt());
        f->loadFromConfig("Functions/IntegralFunction/"+functionId);
        addFunction(f);
    }

    foreach(AbstractFunction *f, mFunctions) {
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
