#include "functionfactory.h"
#include "functionssingleton.h"
#include "function.h"
#include "projectsingleton.h"

#include <QSettings>

FunctionFactory::FunctionFactory()
{
}

// ============================ STATIC PUBLIC ===============================
Function* FunctionFactory::createFromConfig(const QString &functionGroup)
{

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(functionGroup);
    int id = settings->value("id", -1).toInt();

    Function *function = new Function(id);
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
