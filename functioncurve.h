#ifndef FUNCTIONCURVE_H
#define FUNCTIONCURVE_H

#include "curve.h"

class AbstractFunction;

class FunctionCurve : public Curve
{
public:
    FunctionCurve();
    void setFunction(AbstractFunction *f);
    void setComputeRange(double min, double max, int resolution);

private:
    void updateData();

private:
    AbstractFunction *mFunction;
    float mMin, mMax;
};

#endif // FUNCTIONCURVE_H
