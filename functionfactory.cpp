#include "functionfactory.h"
#include "functionssingleton.h"
#include "function.h"

FunctionFactory::FunctionFactory()
{
}

// ============================ STATIC PUBLIC ===============================
Function* FunctionFactory::createFromConfig(const QString &functionGroup)
{
    Function *function = new Function();
    function->loadFromConfig(functionGroup);
    return function;
}

Function* FunctionFactory::createFromFunction(Function *f)
{
    return new Function(*f);
}

Function* FunctionFactory::createFromSingleton(const QString &name)
{
    AbstractFunction *af = Singleton<FunctionsSingleton>::Instance().getFunction(name);
    if(af != 0) {
        Function *f = dynamic_cast<Function*>(af);
        if(f != 0) {
            return new Function(*f);
        }
    }
    return 0;
}
