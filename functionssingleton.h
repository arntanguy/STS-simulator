#ifndef FUNCTIONSSINGLETON_H
#define FUNCTIONSSINGLETON_H

#include <QMap>
#include "singleton.h"
#include "abstractfunction.h"
//class Function;

class FunctionsSingleton
{
public:
    FunctionsSingleton();
    ~FunctionsSingleton();

    void addFunction(AbstractFunction *f);
    AbstractFunction* getFunction(const QString &name);
    QStringList getFunctionNames() const;

    bool hasFunction(const QString& name) const {
        return mFunctions.find(name) != mFunctions.end();
    }

    void loadFromSettings();
    void save();

private:
    // Function name, function
    QMap<QString, AbstractFunction *> mFunctions;
};

#endif // FUNCTIONSSINGLETON_H
