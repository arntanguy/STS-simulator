#include "functioncurve.h"
#include "abstractfunction.h"
#include "muParser.h"

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
// XXX: FIXME: Huge value make program crash (std::bad_alloc)
void FunctionCurve::updateData()
{
    if(mFunction != 0) {
        qDebug() << "FunctionCurve::updateData()";
        int resolution = getResolution();
        QVector<double> x(resolution), y(resolution);
        double stepSize = (mMax-mMin)/resolution;
        qDebug() << "FunctionCurve::updateData() - updating data of " << mFunction->getName()
                 << " with resolution: "<<resolution<<", stepsize: "<<stepSize<<", min: "<<mMin<<", max: "<<mMax;
        double xval = mMin;
        for(int i=0; i<resolution; i++) {
            double yval = 0;
            try {
                yval = mFunction->compute(xval);
            } catch(mu::Parser::exception_type &e) {
                qDebug() << "FunctionCurve::updateData() - ERROR computing function " << mFunction->getName();
                qDebug() << "Message:  " << e.GetMsg().c_str() << "\n";
                qDebug() << "Formula:  " << e.GetExpr().c_str() << "\n";
                qDebug() << "Token:    " << e.GetToken().c_str() << "\n";
                qDebug() << "Position: " << e.GetPos() << "\n";
                qDebug() << "Errc:     " << e.GetCode() << "\n";
                break;
            }
            x.append(xval);
            y.append(yval);
            xval += stepSize;
        }
        setSamples(x, y);
    } else {
        qDebug() << "FunctionCurve::updateData() - error, null function";
    }
}

void FunctionCurve::updateName(const QString &name) {
    setTitle(name);
}
