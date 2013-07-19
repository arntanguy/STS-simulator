#ifndef FUNCTIONCURVE_H
#define FUNCTIONCURVE_H

#include "curve.h"

class AbstractFunction;

class FunctionCurve : public Curve
{
    Q_OBJECT

public:
    FunctionCurve();
    void setFunction(AbstractFunction *f);
    void setComputeRange(double min, double max, int resolution);

    virtual void update();

public Q_SLOTS:
    void updateData();
    void updateName(const QString &);

private:
    AbstractFunction *mFunction;
    float mMin, mMax;
    QVector<double> mXData, mYData;
};

#endif // FUNCTIONCURVE_H
