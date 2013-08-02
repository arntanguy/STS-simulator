#ifndef FUNCTIONFACTORY_H
#define FUNCTIONFACTORY_H

class Function;

#include <QString>

class FunctionFactory
{
public:
    FunctionFactory();
    static Function* createFromConfig(const QString &functionGroup);
    static Function* createFromSingleton(const QString &name);
    static Function* createFromFunction(Function *f);
};

#endif // FUNCTIONFACTORY_H
