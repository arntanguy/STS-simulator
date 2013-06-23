#include "function.h"

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
              << a_szName << std::endl;

    // Use the variable factory passed as user data to create a new variable
    VariableFactory *varFactory= static_cast<VariableFactory*>(pUserVariableFactory);
    varFactory->createNewVariable(a_szName);

    *varFactory->getVariableAddress(a_szName) = 2;
    return varFactory->getVariableAddress(a_szName);
}


Function::Function(QObject *parent) : QObject(parent)
{
    mVariable = "V";

    // Defines the variable factory used for implicit variable declaration.
    mParser.SetVarFactory(addVariable, &mImplicitVarFactory);
}


void Function::setImplicitVariable(const QString &varName, double value)
{
    if(mImplicitVarFactory.hasVariable(varName)) {
        mImplicitVarFactory.setValue(varName, value);
    }
}
