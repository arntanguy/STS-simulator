#include "functionfactory.h"
#include "functionssingleton.h"
#include "function.h"
#include "projectsingleton.h"

#include <QSettings>
#include <QDebug>

FunctionFactory::FunctionFactory()
{
}

// ============================ STATIC PUBLIC ===============================
FunctionPtr FunctionFactory::createFromConfig(const QString &functionGroup)
{

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(functionGroup);
    qDebug() << "FunctionFactory " << settings->group();
    int id = settings->value("id", -1).toInt();
    settings->endGroup();

    Function *function = new Function(id);
    function->loadFromConfig(functionGroup);
    return FunctionPtr(function);
}

FunctionPtr FunctionFactory::createFromFunction(const FunctionPtr &f)
{
    return FunctionPtr(new Function(*(f.data())));
}

//Function* FunctionFactory::createFromSingleton(int id)
//{
//    QSharedPointer<AbstractFunction> af = Singleton<FunctionsSingleton>::Instance().getFunctionById(id);
//    if(af != 0) {
//        Function *f = dynamic_cast<Function*>(af.data());
//        if(f != 0) {
//            return new Function(*f);
//        }
//    }
//    return 0;
//}
