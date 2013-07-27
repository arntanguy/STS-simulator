#ifndef FUNCTION_H
#define FUNCTION_H

#include "abstractfunction.h"

#include <QObject>
#include <QString>
#include <QMetaType>


class VariableFactory;
namespace mu
{
    class Parser;
}

double* addVariable(const char *a_szName, void *pUserVariableFactory);

class Function : public AbstractFunction
{
    Q_OBJECT

public:
    Function(QObject *parent = 0);
    Function(Function const& toCopy);
    ~Function();

    void setExpression(const QString &exp);
    QString getExpression() const;
    bool isValidExpression() const;
    QString getError() const;

    void setImplicitVariable(const QString &varName, double value);
    VariableFactory* getVariableFactory() {
        return mImplicitVarFactory;
    }
    QString getVariable() const {
        return AbstractFunction::getVariable();
    }
    bool hasVariable(const QString &name) const;
    double* getVariable(const QString &name);

    bool setParameters(const QString &parameters);
    QString getParameters() const;


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

private:
    void loadVariables(const QString &group);
Q_SIGNALS:
    void nameUpdated();
    void expressionChanged();
    void needsRecompute();

private:
    void init();
    void cleanupVariables();
    double compute(const QString& parameters, double x);
    bool checkParameters(const QString &parameters) const;
    double computeOnly(double x);

private:
    QString mParameters;
protected:
    mu::Parser *mParser;

    // Implicit variables factory
    VariableFactory *mImplicitVarFactory;
};

Q_DECLARE_METATYPE(Function*);

#endif // FUNCTION_H
