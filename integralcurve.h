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

    virtual void update(bool forceUpdate = false);
    virtual void save();
    virtual void loadFromSettings();

public Q_SLOTS:
    void initGlobalSettings();

private:
    void init();
    virtual void updateData();

private:
    int mStepNumber;
};

#endif // INTEGRALCURVE_H
