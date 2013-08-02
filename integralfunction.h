#ifndef INTEGRALFUNCTION_H
#define INTEGRALFUNCTION_H

#include "hierarchicalfunction.h"

#include <QVector>
#include <QMap>

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
    IntegralFunction(int id);


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
    QString getIntegralExpression() const;

    virtual double compute(double);
    virtual double compute(const QString&, double);
    virtual IntegralData integrate(double min, double max, double resolution, double stepNumber);
    virtual FunctionCurve* createCurve();

    void setParameters(Function *f, const QString &parameter);
    QString getParameters(Function *f) const;

private:
    void init();
    void setFunctionsIntegrationVariable();

private:
    double mStepNumber;
    Range mRange;

    QString mIntegrationVariable;

    QMap<Function*, QString> mParameters;

};

#endif // INTEGRALFUNCTION_H
