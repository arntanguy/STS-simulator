#ifndef INTEGRALFUNCTION_H
#define INTEGRALFUNCTION_H

#include "hierarchicalfunction.h"

#include <QVector>

struct IntegralData
{
    QVector<double> x;
    QVector<double> y;
};

class IntegralFunction : public HierarchicalFunction
{
public:
    IntegralFunction();


    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);

    void setIntegrationVariable(const QString &variable);
    QString getIntegrationVariable() const;

    virtual double compute(double);
    virtual IntegralData integrate(double min, double max, double resolution, double stepNumber);

private:
    void init();
    void setFunctionsIntegrationVariable();

private:
    double mStepNumber;
    double mLowerVal;
    QString mIntegrationVariable;

};

#endif // INTEGRALFUNCTION_H
