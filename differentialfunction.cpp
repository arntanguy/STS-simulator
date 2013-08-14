#include "differentialfunction.h"
#include "differentialcurve.h"
#include "integralfunction.h"
#include "projectsingleton.h"
#include "functionssingleton.h"
#include <QSettings>

DifferentialFunction::DifferentialFunction() : AbstractFunction()
{
    init();
}

DifferentialFunction::DifferentialFunction(int id) : AbstractFunction(id)
{
    init();
}

void DifferentialFunction::init()
{
    qDebug() << "Init new DifferentialFunction";
    setType(AbstractFunction::Differential);
}


double DifferentialFunction::compute(const QString&, double)
{
    qDebug() << "DifferentialFunction::compute() -- NOT IMPLEMENTED!";
    exit(0);
    return -1;
}
double DifferentialFunction::compute(double)
{
    qDebug() << "DifferentialFunction::compute() -- NOT IMPLEMENTED!";
    exit(0);
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

void DifferentialFunction::save(const QString &group)
{
    qDebug() << "DifferentialFunction::save - saving function " << mName;
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    AbstractFunction::abstractsave(group+"/DifferentialFunction/");

    settings->beginGroup(group+"/DifferentialFunction/"+QString::number(getId()));
    if(mFunction != 0)
        settings->setValue("integralFunctionId", mFunction->getId());
    settings->endGroup();
}

void DifferentialFunction::loadFromConfig(const QString &group)
{
    qDebug() << "==============================================";
    qDebug() << "DifferentialFunction::loadFromConfig -  " << group;
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    AbstractFunction::loadFromConfig(group);

    settings->beginGroup(group);
    int id = settings->value("integralFunctionId", -1).toInt();
    qDebug() << "id: "<<id;
    IntegralFunctionPtr f = qSharedPointerDynamicCast<IntegralFunction>(Singleton<FunctionsSingleton>::Instance().getFunctionById(id));
    if(f != 0) {
        qDebug() << "f isn't null";
        DifferentialFunction::setFunction(f);
    }
    settings->endGroup();
    if(mLinkedCurve != 0) mLinkedCurve->update(true);
}

void DifferentialFunction::setFunction(const IntegralFunctionPtr &f)
{
    if(f != 0) {
        qDebug() << "DifferentialFunction::setFunction ";
        mFunction = f;
        connect(mFunction.data(), SIGNAL(integralDataComputed()), this, SIGNAL(integralDataComputed()));
        if(mLinkedCurve != 0) {
            DifferentialCurve *c = dynamic_cast<DifferentialCurve*>(mLinkedCurve);
            if(c!=0) {
                c->setFunction(this);
            }
        }
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
            double h = 0;
            for(int i=0; i<data.size()-1; i++) {
                double x0 = data.x[i];
                double x1 = data.x[i+1];
                double y0 = data.y[i];
                double y1 = data.y[i+1];
                h = x1-x0;
                if(h != 0) {
                    double d = (y1-y0)/h;
                    derivate.x.append(x0);
                    derivate.y.append(d);
                }
            }
            // Use previous value as last value
            double lastValue = derivate.y[derivate.size() - 1];
            derivate.x.append(data.x[data.size() -1]);
            derivate.y.append(lastValue);
        } else {
            qDebug() << "DifferentialCurve::updateData() -- ERROR: invalid data set: size < 2";
        }
    } else {
        qDebug() << "DifferentialFunction::differentiate() - ERROR: NULL FUNCTION";
    }
    return derivate;
}

