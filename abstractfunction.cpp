#include "abstractfunction.h"
#include "projectsingleton.h"
#include "curvesingleton.h"

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
}

void AbstractFunction::updateLinkedCurve()
{
    if(mLinkedCurve != 0) {
        mLinkedCurve->update();
    }
}

bool AbstractFunction::isDisplayed(unsigned int plotId) const
{
    if(mLinkedCurve != 0)
        return mLinkedCurve->isAttached(plotId);
    else
        return false;
}

// ========================= VIRTUAL =========================
void AbstractFunction::save(const QString &group)
{
    qDebug() << "AbstractFunction::save("<<group<<")";

    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    if(settings != 0)
        qDebug() << "AbstractFunction::save - valid settings" <<mName;
    else
        qDebug() << "AbstractFunction::save - invalid settings" <<mName;
}


void AbstractFunction::loadFromConfig(const QString &group)
{
    qDebug() << "AbstractFunction::loadFromConfig("<<group<<")";
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    unsigned int curveId = settings->value(group+"/curveId", 0).toUInt();
    qDebug() << "AbstractFunction::loadFromConfig() -- curve id "<< curveId << "";
    Curve *c = Singleton<CurveSingleton>::Instance().getCurve(curveId);
    if(c != 0) {
        qDebug() << "AbstractFunction::loadFromConfig() - valid curve";
        FunctionCurve *curve = dynamic_cast<FunctionCurve*>(c);
        if(curve != 0) {
            qDebug() << "AbstractFunction::loadFromConfig() - valid function curve";
            curve->loadFromSettings();
            curve->setFunction(this);
            setCurve(curve);
            updateLinkedCurve();
        }
    }
    settings->endGroup();
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
    if(mLinkedCurve != 0)
        settings->setValue("curveId", mLinkedCurve->getId());
    settings->endGroup();
}
