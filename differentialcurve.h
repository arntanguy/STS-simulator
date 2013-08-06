#ifndef DIFFERENTIALCURVE_H
#define DIFFERENTIALCURVE_H

#include "functioncurve.h"

class DifferentialFunction;

class DifferentialCurve : public FunctionCurve
{
public:
    DifferentialCurve();
    DifferentialCurve(int id);

    virtual void update(bool forceUpdate = false);
    virtual void save();
    virtual void loadFromSettings();

    void setFunction(DifferentialFunction *f);

    //virtual void copyFromCurve(Curve *curve);
    virtual void attach(PlotWidget *plot);

public Q_SLOTS:
    virtual void slotUpdateData();


private:
    void init();
    virtual void updateData();
};

#endif // DIFFERENTIALCURVE_H
