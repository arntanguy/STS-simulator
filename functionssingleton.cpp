#include "functionssingleton.h"

#include <QStringList>

FunctionsSingleton::FunctionsSingleton()
{
}
FunctionsSingleton::~FunctionsSingleton()
{
}

void FunctionsSingleton::addFunction(Function *f)
{
    mFunctions[f->getName()] = f;
}

Function *FunctionsSingleton::getFunction(const QString &name)
{
    return mFunctions[name];
}

QStringList FunctionsSingleton::getFunctionNames() const
{
    return mFunctions.keys();
}
