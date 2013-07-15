#include "functionfactory.h"
#include "functionssingleton.h"

FunctionFactory::FunctionFactory()
{
}

// ============================ STATIC PUBLIC ===============================
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
