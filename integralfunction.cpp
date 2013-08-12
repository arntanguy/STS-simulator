#include <cmath>

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

IntegralFunction::~IntegralFunction()
{
    qDebug() << "Delete integralfunction " << getName();
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
    foreach(FunctionPtr f, mFunctions) {
        exp += f->getExpression() + separator;
    }
    return exp.left(exp.length() - separator.length());
}

QString IntegralFunction::getExpressionParameters() const
{
    QString exp;
    QString separator = " * ";
    foreach(FunctionPtr f, mFunctions) {
        exp += f->getName() + "(" + getParameters(f) + ")" + separator;
    }
    return exp.left(exp.length() - separator.length());
}

QString IntegralFunction::getIntegralExpression() const
{
    if(mFunctions.size() > 0) {
        QString exp;
        QString separator = " * ";
        foreach(FunctionPtr f, mFunctions) {
            exp += f->getName() + "(" + getParameters(f) + ")" + separator;
        }
        return exp.left(exp.length() - separator.length()) + "\t<i>d</i>" + mIntegrationVariable;
    } else {
        return QString();
    }
}

bool IntegralFunction::hasFunction(int id) const
{
    foreach(FunctionPtr f, mFunctions) {
        if(f != 0 && f->getId() == id) {
            return true;
        }
    }
    return false;
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

void IntegralFunction::setParameters(const QSharedPointer<Function> &f, const QString &parameter)
{
    mParameters[f->getId()] = parameter;
    mNeedsUpdate = true;
    emit needsRecompute();
}
QString IntegralFunction::getParameters(const FunctionPtr &f) const
{
    return mParameters[f->getId()];
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
PlotData IntegralFunction::integrate(double min, double max, double resolution, double stepNumber)
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

    mData.clear();
    mData.reserve(resolution);

    if(mRange == ZeroToV) {
        qDebug() << "IntegralFunction::integrate() -- 0 to V";
        double r = 0;
        min = 0;
        double x = min;
        double e = min;

        // Set upper limit (V)
        for(; x<max; x += step) {
            double h1 = 1;
            // XXX: Allow for other operations than *
            foreach(FunctionPtr f, mFunctions) {
                f->setVariable(f->getVariable(), x);
                h1 *= f->computeWithParameters(mParameters[f->getId()], mIntegrationVariable, e);
            }
            while(e < x) {
                // Avoid recomputing h0 needlessly.
                // See midpoint graph for explanation
                double h0 = h1;

                e += deltaX;
                h1 = 1;
                foreach(FunctionPtr f, mFunctions) {
                    f->setVariable(f->getVariable(), x);
                    h1 *= f->computeWithParameters(mParameters[f->getId()], mIntegrationVariable, e);
                }
                r += deltaX * (h0 + h1)/(double)2;
            }
            mData.x.append(x);
            mData.y.append(r);
        }
    } else if(mRange == MinusVToZero) {
        qDebug() << "IntegralFunction::integrate() -- 0 to -V";
        double r = 0;
        max = 0;
        double x = max;
        double e = max;

        // Set upper limit (V)
        for(; x>min; x -= step) {
            double h1 = 1;
            // XXX: Allow for other operations than *
            foreach(FunctionPtr f, mFunctions) {
                f->setVariable(f->getVariable(), x);
                h1 *= f->computeWithParameters(mParameters[f->getId()], mIntegrationVariable, e);
            }
            while(e > x) {
                // Avoid recomputing h0 needlessly.
                // See midpoint graph for explanation
                double h0 = h1;

                e -= deltaX;
                h1 = 1;
                foreach(FunctionPtr f, mFunctions) {
                    f->setVariable(f->getVariable(), x);
                    h1 *= f->computeWithParameters(mParameters[f->getId()], mIntegrationVariable, e);
                }
                r += deltaX * (h0 + h1)/(double)2;
            }
            mData.x.append(x);
            mData.y.append(r);
        }
    } else {
        // XXX:
        qDebug() << "IntegralFunction::integrate() - INVALID RANGE: supported range are 0 to V and 0 to -V";
    }
    qDebug() << "IntegralFunction::integrate() - emitting finish signal integralDataComputed()";
    emit integralDataComputed();
    return mData;
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
    setRange(static_cast<Range>(settings->value("range", ZeroToV).toUInt()));
    QStringList functionIds = settings->value("ids", QStringList()).toStringList();
    QStringList parameters = settings->value("parameters", QStringList()).toStringList();
    for(int i=0; i<functionIds.size(); i++) {
        int id = functionIds[i].toInt();
        QString parameter = parameters[i];
        QSharedPointer<Function> f = qSharedPointerDynamicCast<Function>(Singleton<FunctionsSingleton>::Instance().getFunctionById(id));
        if(f != 0) {
            setParameters(f, parameter);
            addFunction(f);
        } else {
            qDebug() << "================= FATAL Error: function " << id << " doesn't exist!";
        }
    }
    settings->endGroup();
    if(mLinkedCurve != 0) mLinkedCurve->update(true);
}

void IntegralFunction::save(const QString &group)
{
    qDebug() << "IntegralFunction::save - saving function " << mName;
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    AbstractFunction::abstractsave(group+"/IntegralFunction/");

    QString groupName = group+"/IntegralFunction/"+QString::number(getId())+"/";
    QStringList functionIds;
    QStringList parameters;
    foreach(FunctionPtr f, mFunctions) {
        if(f != 0) {
            functionIds << QString::number(f->getId()) ;
            parameters << mParameters[f->getId()];
        }
    }
    settings->beginGroup(groupName);
    qDebug() << "IntegralFunction::save - group name: " << groupName;
    settings->setValue("ids", functionIds);
    settings->setValue("parameters", parameters);
    settings->setValue("range", mRange);
    settings->endGroup();
}
