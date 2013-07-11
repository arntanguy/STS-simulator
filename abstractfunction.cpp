#include "abstractfunction.h"
#include "projectsingleton.h"

#include <QDebug>

AbstractFunction::AbstractFunction()
{
}

void AbstractFunction::setName(const QString &name)
{
    mName = name;
}

QString AbstractFunction::getName() const
{
    return mName;
}

QString AbstractFunction::getVariable() const
{
    return mVariable;
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
