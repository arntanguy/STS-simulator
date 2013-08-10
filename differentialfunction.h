#ifndef DIFFERENTIALFUNCTION_H
#define DIFFERENTIALFUNCTION_H

#include "abstractfunction.h"
#include "plotdata.h"

class DifferentialCurve;
class IntegralFunction;

/**
 * Compute the derivative of an integral function
 **/
class DifferentialFunction : public AbstractFunction
{
    Q_OBJECT
public:
    DifferentialFunction();
    DifferentialFunction(int id);

    virtual double compute(const QString&, double);
    virtual double compute(double);
    virtual QString getExpression() const;
    virtual FunctionCurve* createCurve();
    virtual void save(const QString &group);
    virtual void loadFromConfig(const QString &group);

    PlotData differentiate();

    void setFunction(const IntegralFunctionPtr& f);
    void setCurve(DifferentialCurve *curve);

Q_SIGNALS:
    void integralDataComputed();

private:
    void init();

private:
    IntegralFunctionPtr mFunction;
};

#endif // DIFFERENTIALFUNCTION_H
