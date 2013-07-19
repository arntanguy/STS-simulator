#include "abstractfunction.h"
#include "projectsingleton.h"

#include <QDebug>
#include "plotwidget.h"
#include "functioncurve.h"

AbstractFunction::AbstractFunction(QObject *parent) : QObject(parent)
{
    mBaseGroup = "Functions/AbstractFunction/";
    mLinkedCurve = 0;
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

QString AbstractFunction::getVariable() const
{
    return mVariable;
}

void AbstractFunction::setCurve(FunctionCurve *curve)
{
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
}

void AbstractFunction::updateLinkedCurve()
{
    if(mLinkedCurve != 0) {
        mLinkedCurve->update();
    }
}

// ========================= VIRTUAL =========================
void AbstractFunction::save(const QString &group)
{
    qDebug() << "AbstractFunction::save("<<group<<")";
}

QString AbstractFunction::getGroup() const
{
    return mBaseGroup + mName;
}

// ========================== SLOTS =========================
void AbstractFunction::updateLinkedCurve(QString var, double val)
{
    updateLinkedCurve();
    emit curveUpdated(this);
}

// ========================== PROTECTED =====================
void AbstractFunction::abstractsave(const QString &group)
{
    qDebug() << "AbstractFunction::abstractsave - saving " << mName;
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    if(settings == 0) qDebug() << "null settings";
    settings->beginGroup(group+"/"+mName);
    settings->setValue("name", mName);
    settings->endGroup();
}
