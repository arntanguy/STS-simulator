#ifndef FUNCTIONSSINGLETON_H
#define FUNCTIONSSINGLETON_H

#include <QMap>
#include "singleton.h"

class AbstractFunction;

class FunctionsSingleton
{
public:
    FunctionsSingleton();
    ~FunctionsSingleton();

    void addFunction(AbstractFunction *f);
    void removeFunction(AbstractFunction *f);
    AbstractFunction* getFunctionById(int id);
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
    QMap<int , AbstractFunction *> mFunctions;
};

#endif // FUNCTIONSSINGLETON_H
