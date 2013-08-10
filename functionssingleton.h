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
    void removeFunction(const AbstractFunctionPtr &f);
    AbstractFunctionPtr getFunctionById(int id);
    QList<int> getFunctionIds() const;

    bool hasFunction(int id) const {
        return mFunctions.find(id) != mFunctions.end();
    }
    bool functionNameExists(const QString& name) const;

    void loadFromSettings();
    void save();
    void clear();

private:
    // Function name, function
    QMap<int , AbstractFunctionPtr> mFunctions;
};

#endif // FUNCTIONSSINGLETON_H
