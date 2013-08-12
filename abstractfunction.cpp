#include "abstractfunction.h"
#include "projectsingleton.h"
#include "curvesingleton.h"

#include <QDebug>
#include <algorithm>
#include "plotwidget.h"
#include "functioncurve.h"

int AbstractFunction::mFunctionStaticId = 0;
AbstractFunction::FunctionType operator|(AbstractFunction::FunctionType a, AbstractFunction::FunctionType b)
{
    return static_cast<AbstractFunction::FunctionType>(static_cast<int>(a) | static_cast<int>(b));
}


AbstractFunction::AbstractFunction(QObject *parent) : QObject(parent)
{
    mLinkedCurve = 0;
    setId(mFunctionStaticId);
}

AbstractFunction::AbstractFunction(int id, QObject *parent) : QObject(parent)
{
    mLinkedCurve = 0;
    setId(id);
}

AbstractFunction(const AbstractFunction&)
{
    mLinkedCurve = 0;
    setId(mFunctionStaticId);
}

void AbstractFunction::setId(int id)
{
    mFunctionId = id;
    mFunctionStaticId = std::max(id, mFunctionStaticId) + 1;
}

void AbstractFunction::setName(const QString &name)
{
    if(name != mName) {
        mName = name;
        emit nameUpdated(mName);
    }
}

QString AbstractFunction::getName() const
{
    return mName;
}

void AbstractFunction::setType(AbstractFunction::FunctionType type)
{
    mType = type;
}
AbstractFunction::FunctionType AbstractFunction::getType() const
{
    return mType;
}

QString AbstractFunction::getTypeStr() const
{
    switch(getType()) {
        case Function:
            return tr("Base Function");
        case Integral:
            return tr("Integral Function");
        case Differential:
            return tr("Differential Function");
        case HierarchicalFunction:
            return tr("Hierarchical Function");
        default:
            return tr("Invalid");
    }
}

QString AbstractFunction::getVariable() const
{
    return mVariable;
}

void AbstractFunction::setCurve(FunctionCurve *curve)
{
    qDebug() << "AbstractFunction::setCurve()";
    mLinkedCurve = curve;
}

FunctionCurve* AbstractFunction::createCurve()
{
    if(mLinkedCurve == 0) {
        mLinkedCurve = new FunctionCurve();
        mLinkedCurve->setTitle(mName);
        mLinkedCurve->setFunction(this);
    }
    return mLinkedCurve;
}

FunctionCurve* AbstractFunction::getCurve()
{
    return mLinkedCurve;
}

unsigned int AbstractFunction::getCurveId() const
{
    if(mLinkedCurve != 0) {
        return mLinkedCurve->getId();
    }
    return -1;
}

void AbstractFunction::updateLinkedCurve(bool forceUpdate)
{
    qDebug() << "AbstractFunction::updateLinkedCurve";
    if(mLinkedCurve != 0) {
        mLinkedCurve->update(forceUpdate);
    }
}

bool AbstractFunction::isDisplayed(unsigned int plotId) const
{
    if(mLinkedCurve != 0)
        return mLinkedCurve->isAttached(plotId);
    else
        return false;
}

// PROTECTED
void AbstractFunction::abstractsave(const QString &group)
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(group+"/"+QString::number(mFunctionId));
    settings->setValue("name", mName);
    settings->setValue("id", mFunctionId);
    if(mLinkedCurve != 0)
        settings->setValue("curveId", mLinkedCurve->getId());
    settings->endGroup();
}

// ========================= VIRTUAL =========================
void AbstractFunction::save(const QString &group)
{
}


void AbstractFunction::loadFromConfig(const QString &group)
{
    qDebug() << "AbstractFunction::loadFromConfig("<<group<<")";
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup(group);
    int curveId = settings->value("curveId", -1).toInt();
    setName(settings->value("name", "Unkown").toString());
    settings->endGroup();
    if(curveId != -1) {
        Curve *c = Singleton<CurveSingleton>::Instance().getCurve(curveId);
        if(c != 0) {
            FunctionCurve *curve = dynamic_cast<FunctionCurve*>(c);
            if(curve != 0) {
                curve->setFunction(this);
                setCurve(curve);
            }
        }
    }
}

double AbstractFunction::operator() (double x)
{
    return compute(x);
}
// ========================== SLOTS =========================
void AbstractFunction::updateLinkedCurve(QString var, double val, bool forceUpdate)
{
    emit needsRecompute();
    updateLinkedCurve(forceUpdate);
    emit functionUpdated();
}
















void AbstractFunction::resetStaticId()
{
    mFunctionStaticId = 0;
}
