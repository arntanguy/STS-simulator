#include "abstractfunction.h"
#include "projectsingleton.h"

#include <QDebug>
#include "plotwidget.h"
#include "functioncurve.h"

AbstractFunction::AbstractFunction(QObject *parent) : QObject(parent)
{
    mBaseGroup = "Functions/AbstractFunction/";
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
    // XXX: don't update all curves
    // XXX: don't update all plots

    qDebug() << "XXX: updating all curves";

    foreach(PlotWidget *plot, mLinkedCurves.keys()) {
        qDebug() << "XXX: updating curve " << mLinkedCurves[plot]->title().text();
        mLinkedCurves[plot]->updateData();
        plot->replot();
    }
    //if(mLinkedCurves[var] != 0) {
    //    mLinkedCurves->updateData();
    //}
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
