#include "abstractfunction.h"

AbstractFunction::AbstractFunction()
{
}

void AbstractFunction::setName(const QString &name)
{
    mName = name;
}

QString AbstractFunction::getName() const
{
    return mName;
}

QString AbstractFunction::getVariable() const
{
    return mVariable;
}
