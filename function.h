#ifndef FUNCTION_H
#define FUNCTION_H

#include "muParser.h"
#include "variablefactory.h"
#include "abstractfunction.h"

#include <QObject>
#include <QString>
#include <QMetaType>

double* addVariable(const char *a_szName, void *pUserVariableFactory);

class Function : public AbstractFunction
{
public:
    Function();
    ~Function();

    void setExpression(const QString &exp) {
        mParser.SetExpr(exp.toStdString());
    }

    QString getExpression() const {
        return mParser.GetExpr().c_str();
    }

    void setImplicitVariable(const QString &varName, double value);
    VariableFactory* getVariableFactory() {
        return mImplicitVarFactory;
    }


    bool isValidExpression() const {
        try {
            mParser.Eval();
        } catch (...) {
            return false;
        }
        return true;
    }

    double compute(double x);

    double operator() (double x) {
        return compute(x);
    }

private:
    void init();

private:
    mu::Parser mParser;

    // Implicit variables factory
    VariableFactory *mImplicitVarFactory;
};

Q_DECLARE_METATYPE(Function*);

#endif // FUNCTION_H
