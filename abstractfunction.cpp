#include "abstractfunction.h"
#include "projectsingleton.h"

#include <QDebug>
#include "plotwidget.h"
#include "functioncurve.h"

AbstractFunction::AbstractFunction(QObject *parent) : QObject(parent)
{
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

void AbstractFunction::addCurve(PlotWidget *plot, FunctionCurve *curve)
{
    mLinkedCurves[plot] = curve;
}

FunctionCurve* AbstractFunction::getCurve(PlotWidget *plot)
{
    return mLinkedCurves[plot];
}

// Virtual
void AbstractFunction::save(const QString &group)
{
    qDebug() << "AbstractFunction::save("<<group<<")";
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
