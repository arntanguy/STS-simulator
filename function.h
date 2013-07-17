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
    Q_OBJECT

public:
    Function(QObject *parent = 0);
    Function(Function const& toCopy);
    ~Function();

    void setExpression(const QString &exp);
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

    QString getError() const {
        try {
            mParser.Eval();
        } catch(mu::ParserError &e) {
            return e.GetMsg().c_str();
        } catch(...) {
        }
    }

    double compute(double x);

    double operator() (double x) {
        return compute(x);
    }

    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);

    void setBaseGroup(const QString &baseGroup) {
        mBaseGroup = baseGroup;
    }

// ============ HELPER FUNCTIONS maily for debug ===========================
public:
    void listAllVariables() const;

Q_SIGNALS:
    void nameUpdated();
    void expressionChanged();
    void needsRecompute();

private:
    void init();
    void cleanupVariables();

private:
    mu::Parser mParser;

    // Implicit variables factory
    VariableFactory *mImplicitVarFactory;
};

Q_DECLARE_METATYPE(Function*);

#endif // FUNCTION_H
