#ifndef FUNCTION_H
#define FUNCTION_H

#include "abstractfunction.h"

#include <QObject>
#include <QString>
#include <QMetaType>
#include "typedefs.h"


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
    Function(int id, QObject *parent = 0);
    Function(Function const& toCopy);
    virtual ~Function();

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
    void setVariable(const QString &name, double value);
    double* getVariable(const QString &name);
    QStringList getVariableList() const;
    QStringList getVariablesValueList() const;

    double compute(double x);
    double compute(const QString &variable, double x);
    double computeParameters(const QString& parameters, double max, double integrationValue);

    double operator() (double x) {
        return compute(x);
    }

    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);

// ============ HELPER FUNCTIONS maily for debug ===========================
public:
    void listAllVariables() const;

Q_SIGNALS:
    void nameUpdated();
    virtual void expressionChanged();

private:
    void init();
    void loadVariables();
    void cleanupVariables();

    bool checkParameters(const QString &parameters) const;

protected:
    mu::Parser *mParser;

    // Implicit variables factory
    VariableFactory *mImplicitVarFactory;
};

Q_DECLARE_METATYPE(FunctionPtr);

#endif // FUNCTION_H
