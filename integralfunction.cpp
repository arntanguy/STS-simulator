#include <math.h>

#include <QDebug>
#include "function.h"
#include "integralfunction.h"
#include "integralcurve.h"

IntegralFunction::IntegralFunction() : HierarchicalFunction()
{
    init();
}

void IntegralFunction::init()
{
    setType(mType = Function::Integral);
    mBaseGroup = "Functions/IntegralFunction/";
    setStepNumber(100);
    setRange(ZeroToV);
    mIntegrationVariable = "e";
}

void IntegralFunction::setIntegrationVariable(const QString &variable)
{
    if(variable != mIntegrationVariable) {
        mIntegrationVariable = variable;
        mNeedsUpdate = true;
        updateLinkedCurve();
    }
}
QString IntegralFunction::getIntegrationVariable() const
{
    return mIntegrationVariable;
}

void IntegralFunction::setStepNumber(const int stepNumber)
{
    if(mStepNumber != stepNumber) {
        mStepNumber = stepNumber;
        mNeedsUpdate = true;
    }
}

int IntegralFunction::getStepNumber() const
{
    return mStepNumber;
}

void IntegralFunction::setRange(Range range)
{
    if(mRange != range) {
        mRange = range;
    }
    mNeedsUpdate = true;
}
IntegralFunction::Range IntegralFunction::getRange() const
{
    return mRange;
}

QString IntegralFunction::getExpression() const
{
    QString exp;
    QString separator = " * ";
    foreach(Function *f, mFunctions) {
        exp += f->getExpression() + separator;
    }
    return exp.left(exp.length() - separator.length());
}

QString IntegralFunction::getExpressionParameters() const
{
    QString exp;
    QString separator = " * ";
    foreach(Function *f, mFunctions) {
        exp += f->getName() + "(" + f->getParameters() + ")" + separator;
    }
    return exp.left(exp.length() - separator.length());
}

FunctionCurve* IntegralFunction::createCurve()
{
    if(mLinkedCurve == 0) {
        mLinkedCurve = new IntegralCurve();
        mLinkedCurve->setTitle(mName);
        mLinkedCurve->setFunction(this);
    }
    return mLinkedCurve;
}

/**
 * Integrate by using optimized midpoint method
 * When multiple functions are set, they will be multiplied together at each integration step
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

    IntegralData data(resolution);
    double r = 0;
    double x = min;
    double e = min;

    // Set upper limit (V)
    for(; x<max; x += step) {
        double h1 = 1;
        // XXX: Allow for other operations than *
        foreach(Function *f, mFunctions) {
            f->setVariable(f->getVariable(), x);
            h1 *= f->computeWithParameters(mIntegrationVariable, e);
        }
        while(e < x) {
            // Avoid recomputing h0 needlessly.
            // See midpoint graph for explanation
            double h0 = h1;

            e += deltaX;
            h1 = 1;
            foreach(Function *f, mFunctions) {
                f->setVariable(f->getVariable(), x);
                h1 *= f->computeWithParameters(mIntegrationVariable, e);
            }
            r += deltaX * (h0 + h1)/2.d;
        }
        data.x.append(x);
        data.y.append(r);
    }
    return data;
}

double IntegralFunction::compute(double x)
{
    qDebug() << "XXX: compute individual integral values undefined. Use integrate(...) instead";
    return -1;
}

double IntegralFunction::compute(const QString&, double)
{
    qDebug() << "XXX: compute individual integral values undefined. Use integrate(...) instead";
    return -1;
}

void IntegralFunction::save(const QString &group)
{
    HierarchicalFunction::save(getGroup());
}

void IntegralFunction::loadFromConfig(const QString &group)
{
    HierarchicalFunction::loadFromConfig(getGroup());
}
