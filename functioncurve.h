#ifndef FUNCTIONCURVE_H
#define FUNCTIONCURVE_H

#include "curve.h"

class AbstractFunction;

class FunctionCurve : public Curve
{
    Q_OBJECT

public:
    FunctionCurve();
    FunctionCurve(unsigned int id);
    void setFunction(AbstractFunction *f);
    void setComputeRange(double min, double max, int resolution);

    virtual void update();
    virtual void save();

public Q_SLOTS:
    void updateData();
    void updateName(const QString &);

Q_SIGNALS:
    void functionUpdated(AbstractFunction *);
    void functionDataUpdated(AbstractFunction *);

private:
    AbstractFunction *mFunction;
    float mMin, mMax;
    QVector<double> mXData, mYData;
};

#endif // FUNCTIONCURVE_H
