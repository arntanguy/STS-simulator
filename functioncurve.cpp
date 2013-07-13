#include "functioncurve.h"
#include "abstractfunction.h"

#include <QDebug>
#include <QVector>

FunctionCurve::FunctionCurve() : Curve()
{
}

void FunctionCurve::setFunction(AbstractFunction *f)
{
    mFunction = f;
    setTitle(f->getName());
}

void FunctionCurve::setComputeRange(double min, double max, int resolution)
{
    mMin = min;
    mMax = max;
    setResolution(resolution);

    updateData();
}

/// =============== PRIVATE ==========================
void FunctionCurve::updateData()
{
    if(mFunction != 0) {
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
