#ifndef INTEGRALFUNCTION_H
#define INTEGRALFUNCTION_H

#include "function.h"

class IntegralFunction : public Function
{
public:
    IntegralFunction();

    virtual double compute(double);

    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);

private:
    void init();
    double integrate(double x);

private:
    double mStepNumber;
    double mLowerVal;
    QString mIntegrationVariable;

};

#endif // INTEGRALFUNCTION_H
