#ifndef FUNCTIONFACTORY_H
#define FUNCTIONFACTORY_H

#include "function.h"

class FunctionFactory
{
public:
    FunctionFactory();
    static Function* createFromConfig(const QString &functionGroup);
    static Function* createFromFunction(Function *f);
    static Function* createFromSingleton(const QString &name);
};

#endif // FUNCTIONFACTORY_H
