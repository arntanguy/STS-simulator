#ifndef FUNCTIONCURVE_H
#define FUNCTIONCURVE_H

#include "curve.h"

class AbstractFunction;

class FunctionCurve : public Curve
{
    Q_OBJECT

public:
    FunctionCurve();
    FunctionCurve(int id);
    virtual ~FunctionCurve();

    void setFunction(AbstractFunction *f);
    AbstractFunction* getFunction() {
        return mFunction;
    }
    void setComputeRange(double min, double max, int resolution);

    virtual void update(bool forceUpdate = false);
    virtual void save();
    virtual void loadFromSettings();

    void setResolution(int resolution);
    int getResolution() const;

    virtual void copyFromCurve(Curve *curve);

public Q_SLOTS:
    void initGlobalSettings();

private:
    void init();

protected:
    virtual void updateData();

public Q_SLOTS:
    virtual void slotUpdateData();
    void updateName(const QString &);
    void slotDeferedUpdate();

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
