#include "function.h"
#include "projectsingleton.h"
#include "variablefactory.h"
#include "muParser.h"
#include "functioncurve.h"
#include "helperfunctions.h"

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
    //std::cout << "Generating new variable \""
    //    << a_szName << "\"" << std::endl;

    // Use the variable factory passed as user data to create a new variable
    VariableFactory *varFactory= static_cast<VariableFactory*>(pUserVariableFactory);
    varFactory->createNewVariable(a_szName);

    *varFactory->getVariableAddress(a_szName) = 0;
    return varFactory->getVariableAddress(a_szName);
}

Function::Function(QObject *parent) : AbstractFunction(parent)
{
    init();
}

Function::Function(int id, QObject *parent) : AbstractFunction(id, parent)
{
    init();
}

Function::Function(Function const& toCopy) : AbstractFunction(toCopy)
{
    // Creates a new variable factory and set default parameters
    init();

    setName(toCopy.getName());
    setExpression(toCopy.getExpression());
    mVariable = toCopy.mVariable;
    mType = toCopy.mType;
}

Function::~Function()
{
    qDebug() << "Delete function " << getName();
    delete mImplicitVarFactory;
    delete mParser;
}

// ================================= PRIVATE =================================
void Function::init()
{
    setType(AbstractFunction::Function);

    mParser = HelperFunctions::createParser();
    mImplicitVarFactory = new VariableFactory();
    mVariable = "V";

    // Defines the variable factory used for implicit variable declaration.
    mParser->SetVarFactory(addVariable, mImplicitVarFactory);

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
    mParser->ClearVar();
    // Evaluate new expressions
    mParser->Eval();

    /**
     * Queries all used variable in expression
     **/
    // Get the map with the variables
    varmap_type variables = mParser->GetVar();

    varmap_type::const_iterator item = variables.begin();
    QStringList used;
    for (; item!=variables.end(); ++item)
    {
        used << QString(item->first.c_str());
    }
    //qDebug() << "Function::cleanupVariables() - keeping " << used;
    // Remove all unused variables from the factory
    mImplicitVarFactory->deleteOthers(used);
}

// ===================================== PUBLIC =========================================
void Function::setExpression(const QString &exp)
{
    if(exp.toStdString() != mParser->GetExpr()) {

        mParser->SetExpr(exp.toStdString());

        if(isValidExpression()) {
            cleanupVariables();
        }

        emit expressionChanged();
        emit needsRecompute();
    }
}

QString Function::getExpression() const {
    return mParser->GetExpr().c_str();
}

bool Function::isValidExpression() const {
    try {
        mParser->Eval();
    } catch (...) {
        return false;
    }
    return true;
}

QString Function::getError() const {
    try {
        mParser->Eval();
    } catch(mu::ParserError &e) {
        return e.GetMsg().c_str();
    } catch(...) {
    }
}

void Function::setImplicitVariable(const QString &varName, double value)
{
    double *var = mImplicitVarFactory->getVariableAddress(varName);
    if(var != 0 && *var != value) {
        mImplicitVarFactory->setValue(varName, value);
        emit needsRecompute();
    }
}

/**
 * @brief Function::compute
 *  Computes the value (with parameters if set)
 *  If mParameters (set by using setParameters) is not null, the parameters will be used
 * @param x
 *  Value for which to compute
 * @return
 *  Computed value f(parameters)
 */
double Function::compute(double x)
{
    compute(mVariable, x);
}

double Function::compute(const QString &variable, double x)
{
    //qDebug() << "Function::compute(" << variable <<" = "<<x<<")";
    mParser->DefineVar(variable.toStdString(), &x);
    return mParser->Eval();
}



double Function::computeParameters(const QString& parameters, double max, double integrationValue)
{
    // Hierarchichal function!!! Can't have access to the variables, but we don't need them!
    /**
     * Create a parser for the parameters
     **/
    mu::Parser *pParser = HelperFunctions::createParser();
    pParser->SetExpr(parameters.toStdString());
    VariableFactory pVarFact;
    pParser->SetVarFactory(addVariable, &pVarFact);
    pParser->Eval();

    pVarFact.setValue("e", integrationValue);
    pVarFact.setValue("V", max);

    double parameter = pParser->Eval();
    delete pParser;
    return parameter;
}

/**
 * @brief Function::checkParameters
 *  Checks if the parameter variables are in the expression
 * @return
 *  True if the parameters are valid
 *  False otherwise
 **/
bool Function::checkParameters(const QString &parameters) const
{
    if(parameters.isEmpty()) {
        return true;
    }

    /**
     * Create a parser for the parameters
     **/
    mu::Parser pParser;
    pParser.SetExpr(parameters.toStdString());
    VariableFactory pVarFact;
    pParser.SetVarFactory(addVariable, &pVarFact);
    pParser.Eval();

    /**
     * This loop check if parameters variables are in the function expression
     **/
    // Get the map with the variables
    varmap_type variables = pParser.GetVar();
    varmap_type::const_iterator item = variables.begin();
    // Query the variables
    for (; item!=variables.end(); ++item)
    {
        if(!hasVariable(item->first.c_str())) {
            return false;
        }
    }
    return true;
}

bool Function::hasVariable(const QString &name) const
{
    varmap_type variables = mParser->GetVar();
    return mParser->GetVar().find(name.toStdString()) != variables.end();
}

void Function::setVariable(const QString &name, double value)
{
    double *var = getVariable(name);
    if(var != 0) {
        *var = value;
    } else {
        // XXX: handle error
        qDebug() << "Function::setVariable() - var " << name << " doesn't exist, evaluating to find missing variables";
        mParser->Eval();
        setVariable(name, value);
        //double *val = new double;
        //*val = value;

        //mParser->DefineVar(name.toStdString(), val);
    }
}

double* Function::getVariable(const QString &name)
{
    //varmap_type variables = mParser->GetVar();
    //varmap_type::iterator it = variables.find(name.toStdString());
    //if(it != variables.end()) {
    //    return it->second;
    //} else {
    //    return 0;
    //}
    return mImplicitVarFactory->getVariableAddress(name);
}

QStringList Function::getVariableList() const
{
    QStringList list;
    foreach(QString name, mImplicitVarFactory->getVariableNames()) {
        if(name != getVariable())
            list << name;
    }
    return list;
}

QStringList Function::getVariablesValueList() const
{
    QStringList list;
    foreach(QString name, mImplicitVarFactory->getVariableNames()) {
        if(name != getVariable())
            list << QString::number(*mImplicitVarFactory->getVariableAddress(name));
    }
    return list;
}

// ============================= VIRTUAL =================================
void Function::loadFromConfig(const QString &group)
{
    qDebug() << "Function::loadFromConfig("<<group<<")";
    AbstractFunction::loadFromConfig(group);

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(group);
    setExpression(settings->value("expression", "").toString());
    settings->endGroup();
    loadVariables();
    if(mLinkedCurve != 0)
    {
        mLinkedCurve->update(true);
    }
}

void Function::loadVariables()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Variables/"+QString::number(getId()));
    qDebug() << "Function::loadVariables() -- from group " << settings->group();

    QStringList variables = getVariableFactory()->getVariableNames();
    foreach(QString var, variables) {
        // If it's not the function variable
        if(var != getVariable()) {
            qDebug() << " -------------- Loading variable "<<var<< " = " << settings->value(var+"/value", -1).toDouble();
            getVariableFactory()->setValue(var, settings->value(var+"/value", -1).toDouble());
        }
    }
    settings->endGroup();
}

void Function::save(const QString &group)
{
    qDebug() << "Function::save - Saving function "; //<<mName;
    AbstractFunction::abstractsave(group+"/Function");
    qDebug() << "Function::save - there" <<mName;

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    if(settings != 0)
        qDebug() << "Function::save - valid settings" <<mName;
    else
        qDebug() << "Function::save - invalid settings" <<mName;
    settings->beginGroup(group+"/Function/"+QString::number(getId())+"/");
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
