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
    virtual void loadFromSettings();

    void setResolution(int resolution);
    int getResolution() const;

private:
    void init();
    virtual void updateData();

public Q_SLOTS:
    void slotUpdateData();
    void updateName(const QString &);

Q_SIGNALS:
    void functionUpdated(AbstractFunction *);
    void functionDataUpdated(AbstractFunction *);

protected:
    QVector<double> mXData, mYData;
    AbstractFunction *mFunction;

private:
    int mResolution;
};

#endif // FUNCTIONCURVE_H
