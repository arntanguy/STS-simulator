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


    double result = 1;
    double x = min;
    foreach(AbstractFunction *f, mFunctions) {
        //Function *f = dynamic_cast<Function *>(af);
        if(f != 0) {
            double e = min;
            f->setImplicitVariable(mIntegrationVariable.toStdString().c_str(), &e);
            double r = 0;
            for(; x<max; x += step) {
                while(e < x) {
                    double h0 = f->compute(x);
                    e += deltaX;
                    double h1 = f->compute(x);
                    r += deltaX * (h0 + h1)/2.d;
                }
            }
            // XXX: provide possibility of other operations
            result *= r;
        } else {
            qDebug() << "IntegralFunction::integrate() - CRITICAL ERROR: function isn't of type Function";
        }
    }
    data.x.append(x);
    data.y.append(result);
    //mParser.DefineVar(mVariable.toStdString(), &x);
    //mParser.DefineVar(mIntegrationVariable.toStdString(), &e);

    //for(; x<max; x += step) {
    //    while(e < x) {
    //        double h0 = mParser.Eval();
    //        e += deltaX;
    //        double h1 = mParser.Eval();
    //        result += deltaX * (h0 + h1)/2.d;
    //    }
    //    data.x.append(x);
    //    data.y.append(result);
    //}

    return data;
}

double IntegralFunction::compute(double x)
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
