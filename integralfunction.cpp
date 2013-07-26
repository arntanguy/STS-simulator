#include "integralfunction.h"
#include <math.h>

#include <QDebug>

IntegralFunction::IntegralFunction() : Function()
{
    init();
}

void IntegralFunction::init()
{
    setType(mType = AbstractFunction::Integral);
    mBaseGroup = "Functions/IntegralFunction/";
    mStepNumber = 100;
    mLowerVal = 0;
    mIntegrationVariable = "e";
}

/**
 * Integrate by using optimized midpoint method
 * @param min
 *  Min x value
 * @param max
 *  Max x value
 * @param resolution
 *  Number of points to calculate I(V=min), .... , I(V=max)
 * @param stepNumber
 *  Number of step for each integral computation (i.e, there will be stepNumber steps
 *  between min, and min + (max-min)/resolution
 **/
IntegralData IntegralFunction::integrate(double min, double max, double resolution, double stepNumber)
{
    qDebug() << "IntegralFunction::integrate(min: "<<min<<", max: "<<max<<", resolution: "<<resolution<<", stepNumber: "<<stepNumber<<")";

    if(resolution == 0 || stepNumber == 0) {
        qDebug() << "FATAL ERROR: NULL STEP NUMBER";
        exit(1);
    }

    double step = std::abs(max-min)/resolution;
    double deltaX = (double)(std::abs(max-min)/resolution)/(double)stepNumber;
    qDebug() << "IntegralFunction::integrate() -- step size deltaX="<<deltaX;

    IntegralData data;

    double result = 0;
    double x = min;
    double e = min;
    mParser.DefineVar(mVariable.toStdString(), &x);
    mParser.DefineVar(mIntegrationVariable.toStdString(), &e);

    for(; x<max; x += step) {
        while(e < x) {
            double h0 = mParser.Eval();
            e += deltaX;
            double h1 = mParser.Eval();
            result += deltaX * (h0 + h1)/2.d;
        }
        data.x.append(x);
        data.y.append(result);
    }
    return data;
}

double IntegralFunction::compute(double x)
{
    qDebug() << "XXX: compute individual integral values undefined. Use integrate(...) instead";
    return -1;
}

void IntegralFunction::save(const QString &group)
{
    Function::save(getGroup());
}

void IntegralFunction::loadFromConfig(const QString &group)
{
    Function::loadFromConfig(getGroup());
}
