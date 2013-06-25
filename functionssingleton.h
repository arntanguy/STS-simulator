#ifndef FUNCTIONSSINGLETON_H
#define FUNCTIONSSINGLETON_H

#include <QMap>
#include "singleton.h"
#include "function.h"
//class Function;

class FunctionsSingleton
{
public:
    FunctionsSingleton();
    ~FunctionsSingleton();

    void addFunction(Function *f);
    Function* getFunction(const QString &name);
    QStringList getFunctionNames() const;

    bool hasFunction(const QString& name) const {
        return mFunctions.find(name) != mFunctions.end();
    }

private:
    // Function name, function
    QMap<QString, Function *> mFunctions;
};

#endif // FUNCTIONSSINGLETON_H
