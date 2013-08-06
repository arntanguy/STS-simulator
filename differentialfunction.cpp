#include "differentialfunction.h"
#include "differentialcurve.h"
#include "integralfunction.h"

DifferentialFunction::DifferentialFunction()
{
    init();
}

void DifferentialFunction::init()
{
    setType(AbstractFunction::Differential);
}


double DifferentialFunction::compute(const QString&, double)
{
    qDebug() << "DifferentialFunction::compute() -- NOT IMPLEMENTED!";
    return -1;
}
double DifferentialFunction::compute(double)
{
    qDebug() << "DifferentialFunction::compute() -- NOT IMPLEMENTED!";
    return -1;
}
QString DifferentialFunction::getExpression() const
{
    return mFunction->getName()+"'("+mFunction->getVariable()+") = derivate(" + mFunction->getExpression() + ")";
}

FunctionCurve* DifferentialFunction::createCurve()
{
    qDebug() << "DifferentialFunction::createCurve()";
    if(mLinkedCurve == 0) {
        qDebug() << "Create new curve";
        DifferentialCurve *c = new DifferentialCurve();
        c->setTitle(mName);
        c->setFunction(this);
        mLinkedCurve = c;
    }
    return mLinkedCurve;
}

void DifferentialFunction::setFunction(IntegralFunction *f)
{
    if(f != 0) {
        qDebug() << "DifferentialFunction::setFunction ";
        mFunction = f;
        connect(mFunction, SIGNAL(integralDataComputed()), this, SIGNAL(integralDataComputed()));
    }
}

void DifferentialFunction::setCurve(DifferentialCurve *curve)
{
    if(curve != 0) {
        qDebug() << "DifferentialFunction::setCurve(" << curve->title().text() ;
        mLinkedCurve = curve;
    }
}



PlotData DifferentialFunction::differentiate()
{
    PlotData derivate;
    if(mFunction != 0) {
        PlotData data = mFunction->getData();
        if(data.size() >2) {
            derivate.reserve(data.size());
            double x0 = data.x[0];
            double x1 = data.x[1];
            double step = x1-x0;
            double y0 = data.x[0];
            double y1 = data.x[1];
            for (int i = 2; i < data.size(); i+=2 ) {
                double d = (y1-y0)/step;
                derivate.x.append(x0);
                derivate.y.append(d);

                x0 = x1;
                y0 = y1;
                x1 = data.x[i];
                y1 = data.y[i];
            }
        } else {
            qDebug() << "DifferentialCurve::updateData() -- ERROR: invalid data set: size < 2";
        }
    } else {
        qDebug() << "DifferentialFunction::differentiate() - ERROR: NULL FUNCTION";
    }
    return derivate;
}

