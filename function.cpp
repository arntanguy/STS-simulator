#include "function.h"
#include "projectsingleton.h"

#include <QDebug>
#include <iostream>
#include "muParser.h"
using namespace mu;

/*!
 * \brief addVariable
 *  This function is meant to be used by muParser for implicit declaration of variables.
 *  Everytime muParser encounters an unknown token, it will be defined as a token, and linked to a variable
 *  by using the VariableFactory.
 *  Variable values should then be defined and modified by passing though the factory
 * \param a_szName
 *  Name of the unknown token (variable) found by muParser
 * \param pUserVariableFactory
 *  The factory class that manages variable creation
 * \return Address of the newly defined variable
 */
double* addVariable(const char *a_szName, void *pUserVariableFactory)
{
    std::cout << "Generating new variable \""
              << a_szName << "\"" << std::endl;

    // Use the variable factory passed as user data to create a new variable
    VariableFactory *varFactory= static_cast<VariableFactory*>(pUserVariableFactory);
    varFactory->createNewVariable(a_szName);

    *varFactory->getVariableAddress(a_szName) = 2;
    return varFactory->getVariableAddress(a_szName);
}

Function::Function(QObject *parent) : AbstractFunction(parent)
{
    init();
}

Function::Function(Function const& toCopy)
{
    // Creates a new variable factory and set default parameters
    init();

    setName(toCopy.getName());
    setExpression(toCopy.getExpression());
    mVariable = toCopy.mVariable;
    mType = toCopy.mType;
    mBaseGroup = "Functions/Function/";
}

Function::~Function()
{
    delete mImplicitVarFactory;
}

// ================================= PRIVATE =================================
void Function::init()
{
    setType(AbstractFunction::Function);

    mImplicitVarFactory = new VariableFactory();
    mVariable = "V";

    // Defines the variable factory used for implicit variable declaration.
    mParser.SetVarFactory(addVariable, mImplicitVarFactory);

    mBaseGroup = "Functions/Function/";
}

/**
 * @brief: Remove unused variables
 * Variables are automatically defined by the parser through the variablefactory, but there is no way to delete
 * them automatically. We need to only delete the variables that are now useless,
 * for the sake of keeping all the other values intact.
 **/
void Function::cleanupVariables()
{
    // Cleanup parser vars (values will be retained by the variablefactory)
    mParser.ClearVar();
    // Evaluate new expressions
    mParser.Eval();

    /**
     * Queries all used variable in expression
     **/
    // Get the map with the variables
    varmap_type variables = mParser.GetVar();

    varmap_type::const_iterator item = variables.begin();
    QStringList used;
    for (; item!=variables.end(); ++item)
    {
        used << QString(item->first.c_str());
    }
    qDebug() << "Function::cleanupVariables() - keeping " << used;
    // Remove all unused variables from the factory
    mImplicitVarFactory->deleteOthers(used);
}

// ===================================== PUBLIC =========================================
void Function::setExpression(const QString &exp)
{
    if(exp.toStdString() != mParser.GetExpr()) {

        mParser.SetExpr(exp.toStdString());

        if(isValidExpression()) {
            cleanupVariables();
        }

        emit expressionChanged();
        emit needsRecompute();
    }
}

void Function::setImplicitVariable(const QString &varName, double value)
{
    if(mImplicitVarFactory->hasVariable(varName)) {
        qDebug() << "Warning: redining variable "+varName;
    }
    mImplicitVarFactory->setValue(varName, value);
    // XXX: don't emit unless necessary
    emit needsRecompute();
}

double Function::compute(double x)
{
    mParser.DefineVar(mVariable.toStdString(), &x);
    return mParser.Eval();
}

// ============================= VIRTUAL =================================
void Function::loadFromConfig(const QString &group)
{
    qDebug() << "Function::loadFromConfig("<<group<<")";
    AbstractFunction::loadFromConfig(group);
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(group);
    setName(settings->value("name", "Unkown").toString());
    setExpression(settings->value("expression", "").toString());
    settings->endGroup();
}

void Function::save(const QString &group)
{
    qDebug() << "Function::save - Saving function "; //<<mName;
    abstractsave(group+"/Function");
    qDebug() << "Function::save - there" <<mName;

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    if(settings != 0)
        qDebug() << "Function::save - valid settings" <<mName;
    else
        qDebug() << "Function::save - invalid settings" <<mName;
    settings->beginGroup(group+"/Function/"+mName+"/");
    settings->setValue("expression", getExpression());
    settings->endGroup();
}













// ============ HELPER FUNCTIONS maily for debug ===========================
void Function::listAllVariables() const
{
    qDebug() << "Listing all  variables for function " << mName;
    qDebug() << "Function variable " << mVariable;
    qDebug() << "Implicit variables:";
    foreach(QString name, mImplicitVarFactory->getVariableNames()) {
        qDebug() << "\t" << name << " (" << mImplicitVarFactory->getVariableAddress(name) << ") = " << *(mImplicitVarFactory->getVariableAddress(name));
    }
}
