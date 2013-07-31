#ifndef INTEGRALFUNCTION_H
#define INTEGRALFUNCTION_H

#include "hierarchicalfunction.h"

#include <QVector>

struct IntegralData
{
    QVector<double> x;
    QVector<double> y;
    IntegralData(int resolution) {
        x.reserve(resolution);
        y.reserve(resolution);
        for(int i = 0; i<resolution; i++) {
            x.append(0);
            y.append(1);
        }
    }
};

class IntegralFunction : public HierarchicalFunction
{
public:
    enum Range { ZeroToV, MinusVToZero };

public:
    IntegralFunction();


    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);

    void setIntegrationVariable(const QString &variable);
    QString getIntegrationVariable() const;

    void setStepNumber(const int stepNumber);
    int getStepNumber() const;

    void setRange(Range range);
    Range getRange() const;

    QString getExpression() const;
    QString getExpressionParameters() const;

    virtual double compute(double);
    virtual double compute(const QString&, double);
    virtual IntegralData integrate(double min, double max, double resolution, double stepNumber);
    virtual FunctionCurve* createCurve();

private:
    void init();
    void setFunctionsIntegrationVariable();

private:
    double mStepNumber;
    Range mRange;

    QString mIntegrationVariable;

};

#endif // INTEGRALFUNCTION_H
