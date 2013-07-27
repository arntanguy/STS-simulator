#ifndef INTEGRALCURVE_H
#define INTEGRALCURVE_H

#include "functioncurve.h"

class IntegralFunction;

class IntegralCurve : public FunctionCurve
{
    Q_OBJECT;

public:
    IntegralCurve();
    IntegralCurve(unsigned int id);

    void setFunction(IntegralFunction *f);
    void setStepNumber(int stepNumber);
    int getStepNumber() const;

    virtual void update();
    virtual void save();
    virtual void loadFromSettings();

private:
    void init();
    virtual void updateData();

private:
    int mStepNumber;
};

#endif // INTEGRALCURVE_H