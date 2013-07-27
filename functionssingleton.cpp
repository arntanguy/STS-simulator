#include "functionssingleton.h"
#include "projectsingleton.h"
#include "function.h"
#include "hierarchicalfunction.h"
#include "abstractfunction.h"

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
        mFunctions[f->getName()] = f;
    } else {
        qDebug() << "FunctionSingleton::addFunction: CANNOT ADD A NULL FUNCTION";
    }
}

void FunctionsSingleton::removeFunction(AbstractFunction *f)
{
    if(f != 0) {
        QMap<QString, AbstractFunction *>::iterator it = mFunctions.find(f->getName());
        if(it != mFunctions.end()) {
            qDebug() << "FunctionsSingleton::removeFunction() - deleting " << it.value()->getName();
            mFunctions.erase(it);
            delete f;
        }
    }
}

AbstractFunction *FunctionsSingleton::getFunction(const QString &name)
{
    return mFunctions[name];
}

QStringList FunctionsSingleton::getFunctionNames() const
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
        Function *f = new Function();
        f->loadFromConfig("Functions/Function/"+functionId);
        addFunction(f);
    }

    settings->beginGroup("Functions/HierarchicalFunction/");
    QStringList hGroups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, hGroups) {
        HierarchicalFunction *f = new HierarchicalFunction();
        f->loadFromConfig("Functions/HierarchicalFunction/"+functionId);
        addFunction(f);
    }

    foreach(AbstractFunction *f, mFunctions) {
        if(f->getType() == AbstractFunction::HierarchicalFunction) {
            f->updateLinkedCurve();
        }
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
    foreach(QString fName, mFunctions.keys()) {
        qDebug() << "Function " << fName;
        mFunctions[fName]->save("Functions");
    }
}
