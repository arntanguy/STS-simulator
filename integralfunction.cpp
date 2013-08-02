#include <math.h>

#include <QDebug>
#include <QSettings>
#include "projectsingleton.h"
#include "functionssingleton.h"
#include "function.h"
#include "integralfunction.h"
#include "integralcurve.h"

IntegralFunction::IntegralFunction() : HierarchicalFunction()
{
    init();
}

IntegralFunction::IntegralFunction(int id) : HierarchicalFunction(id)
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
        exp += f->getName() + "(" + getParameters(f) + ")" + separator;
    }
    return exp.left(exp.length() - separator.length());
}

QString IntegralFunction::getIntegralExpression() const
{
    if(mFunctions.size() > 0) {
        QString exp;
        QString separator = " * ";
        foreach(Function *f, mFunctions) {
            exp += f->getName() + "(" + getParameters(f) + ")" + separator;
        }
        return exp.left(exp.length() - separator.length()) + "\t<i>d</i>" + mIntegrationVariable;
    } else {
        return QString();
    }
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

void IntegralFunction::setParameters(Function *f, const QString &parameter)
{
    mParameters[f] = parameter;
    mNeedsUpdate = true;
    emit needsRecompute();
}
QString IntegralFunction::getParameters(Function *f) const
{
    return mParameters[f];
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
    qDebug() << "Expression : " << getExpression();

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

    qDebug() << "Here";

    // Set upper limit (V)
    for(; x<max; x += step) {
        double h1 = 1;
        // XXX: Allow for other operations than *
        foreach(Function *f, mFunctions) {
            f->setVariable(f->getVariable(), x);
            qDebug() << mIntegrationVariable << " " << e;
            h1 *= f->computeWithParameters(mParameters[f], mIntegrationVariable, e);
        }
        while(e < x) {
            // Avoid recomputing h0 needlessly.
            // See midpoint graph for explanation
            double h0 = h1;

            e += deltaX;
            h1 = 1;
            foreach(Function *f, mFunctions) {
                f->setVariable(f->getVariable(), x);
                h1 *= f->computeWithParameters(mParameters[f], mIntegrationVariable, e);
            }
            r += deltaX * (h0 + h1)/2.d;
        }
        qDebug() << x << " " << r;
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


void IntegralFunction::loadFromConfig(const QString &group)
{
    qDebug() << "IntegralFunction::loadFromConfig("<<group<<")";
    Function::loadFromConfig(group);
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(group);

    setName(settings->value("name", "Unknown").toString());

    QStringList functionIds = settings->value("ids", QStringList()).toStringList();
    QStringList parameters = settings->value("parameters", QStringList()).toStringList();
    for(int i=0; i<functionIds.size(); i++) {
        QString id = functionIds[i];
        QString parameter = parameters[i];
        Function *f = dynamic_cast<Function *>(Singleton<FunctionsSingleton>::Instance().getFunctionById(id));
        if(f != 0) {
            setParameters(f, parameter);
            addFunction(f);
        } else {
            qDebug() << "================= FATAL Error: function " << id << " doesn't exist!";
        }
    }
    settings->endGroup();
}

void IntegralFunction::save(const QString &group)
{
    qDebug() << "IntegralFunction::save - saving function " << mName;
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    AbstractFunction::abstractsave(group+"/IntegralFunction/");

    QString groupName = group+"/IntegralFunction/"+mName+"/";
    QStringList functionIds;
    QStringList parameters;
    foreach(Function *f, mFunctions) {
        if(f != 0) {
            qDebug() << "+++++++++++ adding function "<<f->getName() << " with group "<< f->getGroup();
            functionIds << f->getFunctionId() ;
            parameters << mParameters[f];
        }
    }
    settings->beginGroup(groupName);
    qDebug() << "IntegralFunction::save - group name: " << groupName;
    settings->setValue("ids", functionIds);
    settings->setValue("parameters", parameters);
    settings->endGroup();
}
