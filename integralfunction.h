#ifndef INTEGRALFUNCTION_H
#define INTEGRALFUNCTION_H

#include "function.h"

#include <QVector>

struct IntegralData
{
    QVector<double> x;
    QVector<double> y;
};

class IntegralFunction : public Function
{
public:
    IntegralFunction();


    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);

    virtual double compute(double);
    virtual IntegralData integrate(double min, double max, double resolution, double stepNumber);

private:
    void init();

private:
    double mStepNumber;
    double mLowerVal;
    QString mIntegrationVariable;

};

#endif // INTEGRALFUNCTION_H
