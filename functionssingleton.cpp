#include "functionssingleton.h"

#include <QStringList>

FunctionsSingleton::FunctionsSingleton()
{
}
FunctionsSingleton::~FunctionsSingleton()
{
}

void FunctionsSingleton::addFunction(AbstractFunction *f)
{
    mFunctions[f->getName()] = f;
}

AbstractFunction *FunctionsSingleton::getFunction(const QString &name)
{
    return mFunctions[name];
}

QStringList FunctionsSingleton::getFunctionNames() const
{
    return mFunctions.keys();
}
