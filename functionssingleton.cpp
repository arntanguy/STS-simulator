#include "functionssingleton.h"
#include "projectsingleton.h"
#include "function.h"
#include "hierarchicalfunction.h"

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
        qDebug() << "FunctionsSingleton::loadFromSettings() - Current group " << settings->group();
        Function *f = new Function();
        f->loadFromConfig("Functions/Function/"+functionId);
        addFunction(f);
    }
    settings->endGroup();


    settings->beginGroup("Functions/HierarchichalFunction/");
    QStringList hGroups = settings->childGroups();
    settings->endGroup();

    foreach(QString functionId, hGroups) {
        qDebug() << "FunctionsSingleton::loadFromSettings() - Current group " << settings->group();
        HierarchicalFunction *f = new HierarchicalFunction();
        f->loadFromConfig("Functions/HierarchichalFunctions/"+functionId);
        addFunction(f);
    }
    settings->endGroup();
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
