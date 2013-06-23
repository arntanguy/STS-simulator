#ifndef FUNCTION_H
#define FUNCTION_H

#include "muParser.h"
#include "variablefactory.h"

#include <QObject>
#include <QString>

double* addVariable(const char *a_szName, void *pUserVariableFactory);

class Function : public QObject
{
    Q_OBJECT
public:
    Function(QObject *parent = 0);
    void setExpression(const QString &exp) {
        mParser.SetExpr(exp.toStdString());
    }

    QString getExpression() const {
        return mParser.GetExpr().c_str();
    }

    double operator() (double x) {
        mParser.DefineVar(mVariable.toStdString(), &x);
        return mParser.Eval();
    }

    void setImplicitVariable(const QString &varName, double value);
private:
    mu::Parser mParser;

    // Implicit variables factory
    VariableFactory mImplicitVarFactory;

    QString mVariable;
};

#endif // FUNCTION_H
