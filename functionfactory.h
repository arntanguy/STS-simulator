#ifndef FUNCTIONFACTORY_H
#define FUNCTIONFACTORY_H

#include "function.h"

class FunctionFactory
{
public:
    FunctionFactory();
    static Function* createFromFunction(Function *f);
};

#endif // FUNCTIONFACTORY_H
