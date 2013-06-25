#ifndef FUNCTION_H
#define FUNCTION_H

#include "muParser.h"
#include "variablefactory.h"

#include <QObject>
#include <QString>
#include <QMetaType>

double* addVariable(const char *a_szName, void *pUserVariableFactory);

class Function : public QObject
{
    Q_OBJECT
public:
    Function(QObject *parent = 0);
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

    void setName(const QString &name) {
        mName = name;
    }
    QString getName() const {
        return mName;
    }
    QString getVariable() const {
        return mVariable;
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

    QString mVariable;
    QString mName;
};

Q_DECLARE_METATYPE(Function*);

#endif // FUNCTION_H
