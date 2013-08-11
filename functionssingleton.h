#ifndef FUNCTIONSSINGLETON_H
#define FUNCTIONSSINGLETON_H

#include <QMap>
#include "singleton.h"

#include "typedefs.h"

#include <QSharedPointer>

class AbstractFunction;

class FunctionsSingleton
{
public:
    FunctionsSingleton();
    ~FunctionsSingleton();

    void addFunction(const AbstractFunctionPtr &f);
    bool removeFunction(const AbstractFunctionPtr &f);
    AbstractFunctionPtr getFunctionById(int id);
    QList<int> getFunctionIds() const;

    bool hasFunction(int id) const {
        return mFunctions.find(id) != mFunctions.end();
    }
    bool functionNameExists(const QString& name) const;
    bool exportVariables(const QString& path);

    void loadFromSettings();
    void save();
    void clear();

    IntegralFunctionPtr isSubFunctionOfIntegral(const AbstractFunctionPtr &f);

private:
    // Function name, function
    QMap<int , AbstractFunctionPtr> mFunctions;
};

#endif // FUNCTIONSSINGLETON_H
