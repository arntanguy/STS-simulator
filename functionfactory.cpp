#include "functionfactory.h"

FunctionFactory::FunctionFactory()
{
}

// ============================ STATIC PUBLIC ===============================
Function* FunctionFactory::createFromFunction(Function *f)
{
    return new Function(*f);
}
