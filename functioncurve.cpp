#include "functioncurve.h"
#include "abstractfunction.h"

#include <QDebug>
#include <QVector>
#include <QObject>

FunctionCurve::FunctionCurve() : Curve()
{
}

void FunctionCurve::setFunction(AbstractFunction *f)
{
    if(f != mFunction) {
        mFunction = f;
        setTitle(f->getName());
        connect(mFunction, SIGNAL(needsRecompute()), this, SLOT(updateData()));
        connect(mFunction, SIGNAL(nameUpdated(const QString &)), this, SLOT(updateName(const QString &)));
        mNeedsUpdate = true;
    }
}

void FunctionCurve::setComputeRange(double min, double max, int resolution)
{
    if(min != mMin || max != mMax || resolution != getResolution()) {
        mMin = min;
        mMax = max;
        setResolution(resolution);
        mNeedsUpdate = true;
    }
}

/// =============== VIRTUAL =========================
void FunctionCurve::update()
{
    if(needsUpdate()) {
        updateData();
    }
}

/// =============== SLOTS ==========================
void FunctionCurve::updateData()
{
    if(mFunction != 0) {
        qDebug() << "FunctionCurve::updateData() - updating data of " << mFunction->getName();
        int resolution = getResolution();
        QVector<double> x(resolution), y(resolution);
        double stepSize = (mMax-mMin)/resolution;
        for(int i=0; i<resolution; i++) {
            double xval = mMin+i*stepSize;
            x.append(xval);
            y.append(mFunction->compute(xval));
            setSamples(x, y);
        }
    } else {
        qDebug() << "FunctionCurve::updateData() - error, null function";
    }
}

void FunctionCurve::updateName(const QString &name) {
    setTitle(name);
}
