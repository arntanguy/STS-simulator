#include <math.h>

#include <QDebug>
#include "function.h"
#include "integralfunction.h"

IntegralFunction::IntegralFunction() : HierarchicalFunction()
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
    //Function *f = dynamic_cast<Function *>(af);
    double e = min;

    // Set upper limit (V)
    for(; x<max; x += step) {
        double h1 = 1;
        // XXX: Allow for other operations than *
        foreach(AbstractFunction *f, mFunctions) {
            f->setVariable(f->getVariable(), &x);
            h1 *= f->compute(mIntegrationVariable, e);
        }
        while(e < x) {
            // Avoid recomputing h0 needlessly.
            // See midpoint graph for explanation
            double h0 = h1;

            e += deltaX;
            h1 = 1;
            foreach(AbstractFunction *f, mFunctions) {
                f->setVariable(f->getVariable(), &x);
                h1 *= f->compute(mIntegrationVariable, e);
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
