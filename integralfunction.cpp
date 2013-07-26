#include "integralfunction.h"
#include <math.h>

#include <QDebug>

IntegralFunction::IntegralFunction() : Function()
{
    init();
}

void IntegralFunction::init()
{
    mBaseGroup = "Functions/IntegralFunction/";
    mStepNumber = 100;
    mLowerVal = 0;
    mIntegrationVariable = "e";
}

/**
 * Integrate by using midpoint method
 * XXX: Works but slow
 **/
double IntegralFunction::integrate(double x)
{
    qDebug() << "IntegralFunction::integrate("<<x<<")";
    if(mStepNumber == 0) {
        qDebug() << "FATAL ERROR: NULL STEP NUMBER";
        exit(1);
    }

    double deltaX = (double)(std::abs(x-mLowerVal))/(double)mStepNumber;
    qDebug() << "IntegralFunction::integrate() -- step size deltaX="<<deltaX;

    mParser.DefineVar(mVariable.toStdString(), &x);

    double result = 0;
    double e = mLowerVal;
    while(e < x) {
        mParser.DefineVar(mIntegrationVariable.toStdString(), &e);
        double h0 = mParser.Eval();
        e += deltaX;
        double h1 = mParser.Eval();
        result += deltaX * (h0 + h1)/2.d;
    }
    return result;
}

double IntegralFunction::compute(double x)
{
    return integrate(x);
}

void IntegralFunction::save(const QString &group)
{
    Function::save(getGroup());
}

void IntegralFunction::loadFromConfig(const QString &group)
{
    Function::loadFromConfig(getGroup());
}
