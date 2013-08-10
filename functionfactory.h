#ifndef FUNCTIONFACTORY_H
#define FUNCTIONFACTORY_H

class Function;

#include <QString>
#include <QSharedPointer>

#include "typedefs.h"

class FunctionFactory
{
public:
    FunctionFactory();
    static FunctionPtr createFromConfig(const QString &functionGroup);
    static FunctionPtr createFromSingleton(int id);
    static FunctionPtr createFromFunction(const FunctionPtr &f);
};

#endif // FUNCTIONFACTORY_H
