#include "variablefactory.h"

VariableFactory::VariableFactory(QObject *parent) : QObject(parent)
{
}

void VariableFactory::createNewVariable(const QString &name)
{
    mVariables[name] = -42;
    emit newVariableCreated(name);
}

void VariableFactory::setValue(const QString &name, double value)
{
    mVariables[name] = value;
    emit variableChanged(name);
}
