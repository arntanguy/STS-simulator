#include "function.h"
#include "projectsingleton.h"
#include "variablefactory.h"
#include "muParser.h"

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

    *varFactory->getVariableAddress(a_szName) = 0;
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
    delete mParser;
}

// ================================= PRIVATE =================================
void Function::init()
{
    setType(AbstractFunction::Function);

    mParser = new mu::Parser();
    mImplicitVarFactory = new VariableFactory();
    mVariable = "V";

    // Defines the variable factory used for implicit variable declaration.
    mParser->SetVarFactory(addVariable, mImplicitVarFactory);
    mParser->DefineConst("_pi", (double)3.14159);

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
    qDebug() << "Function::cleanupVariables() - keeping " << used;
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
    //if(mImplicitVarFactory->hasVariable(varName)) {
    //    qDebug() << "Warning: redifning variable "+varName;
    //}
    double *var = mImplicitVarFactory->getVariableAddress(varName);
    if(var != 0 && *var != value) {
        mImplicitVarFactory->setValue(varName, value);
        // XXX: don't emit unless necessary
        emit needsRecompute();
    }
}

bool Function::setParameters(const QString &parameters)
{
    //if(mParameters != parameters) {
        qDebug() << "Function::setParameters - setting " << parameters;
        mNeedsUpdate = true;
        mParameters = parameters;
        emit needsRecompute();
        return true;
    //}
}
QString Function::getParameters() const
{
    return mParameters;
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


/**
 * @brief Function::compute
 *  Computes the value with parameters f(parameters)
 * @param parameters
 *  Any valid expression composed of constants and variables from the function expression
 *  Ex:
 *  compute("V+2*e", 1) => f(V+2*e) => f(21) = 31
 *
 *  If left empty, compute(x) will be called instead
 * @param x
 *  Value for which to compute
 * @return
 *  Computed value f(parameters)
 */
double Function::computeWithParameters(const QString &variable, double x)
{
    //qDebug() << "Function::compute(parameters: " << mParameters <<", x="<<x<<")";

    // Define x value in function parser
    mParser->DefineVar(variable.toStdString(), &x);

    /**
     * Create a parser for the parameters
     **/
    mu::Parser pParser;
    pParser.SetExpr(mParameters.toStdString());
    VariableFactory pVarFact;
    pParser.SetVarFactory(addVariable, &pVarFact);
    pParser.Eval();

    /**
     * This loop check if parameters variables are in the function expression
     * If they are, the parameter variable is set to the function value
     * Otherwise an exception is (XXX) thrown
     **/
    // Get the map with the variables
    varmap_type variables = pParser.GetVar();
    varmap_type::const_iterator item = variables.begin();
    // Query the variables
    for (; item!=variables.end(); ++item)
    {
        double* pVal = 0;
        pVal = getVariable(item->first.c_str());
        if(pVal != 0) {
            pParser.DefineVar(item->first, pVal);
        } else {
            std::cout << "ERROR: " << item->first << std::endl;
            // XXX: throw error!
            return -1;
        }
        //std::cout << "Variable " << item->first << ", value: " << pParser.GetVar()[item->first] << std::endl;
    }

    double parameterValue = pParser.Eval();
    qDebug() << "parameter value for x="<<x<< ": " << parameterValue;
    return compute(mVariable, parameterValue);

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
        qDebug() << "Function::setVariable() - var " << name << " doesn't exist, creating it";
    }
}

double* Function::getVariable(const QString &name)
{
    varmap_type variables = mParser->GetVar();
    varmap_type::iterator it = variables.find(name.toStdString());
    if(it != variables.end()) {
        return it->second;
    } else {
        return 0;
    }
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

    loadVariables(group);
}

void Function::loadVariables(const QString &group)
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Variables/"+group);
    qDebug() << "Function::loadVariables() -- from group " << settings->group();

    QStringList variables = getVariableFactory()->getVariableNames();
    foreach(QString var, variables) {
        // If it's not the function variable
        if(var != getVariable()) {
            getVariableFactory()->setValue(var, settings->value(var+"/value", -1).toDouble());
        }
    }
    settings->endGroup();
    updateLinkedCurve(true);
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
