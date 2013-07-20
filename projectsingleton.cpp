#include "projectsingleton.h"
#include "curvesingleton.h"
#include "datasingleton.h"
#include "functionssingleton.h"
#include <QDebug>
#include <QStringList>


ProjectSingleton::ProjectSingleton() : QObject()
{
    // Load default configuration
    mSettings = new QSettings();
}

ProjectSingleton::~ProjectSingleton()
{
    qDebug() << "ProjectSingleton::~ProjectSingleton()";
    mSettings->sync();
    delete mSettings;
}

void ProjectSingleton::loadDefaultConfig()
{
    qDebug() << "ProjectSingleton::loadDefaultConfig()";
    mSettings = new QSettings();
}

void ProjectSingleton::createNewProject(const QString &fileName)
{
    if(!mFileName.isEmpty()) {
        // XXX
        qDebug() << "filename isn't empty, a project is open, manage it!";
    } else {
        mFileName = fileName;
        delete mSettings;
        mSettings = new QSettings(fileName, QSettings::IniFormat);
        qDebug() << "Creating new configuration for project " << fileName;
        emit projectChanged();
    }
}

void ProjectSingleton::openProject(const QString& fileName)
{
    if(!mFileName.isEmpty()) {
        // XXX
        qDebug() << "filename isn't empty, a project is open, manage it!";
    }

    mFileName = fileName;
    delete mSettings;
    mSettings = new QSettings(fileName, QSettings::IniFormat);

    // Load all curves
    Singleton<DataSingleton>::Instance().loadFromSettings();
    Singleton<CurveSingleton>::Instance().loadFromSettings();
    Singleton<FunctionsSingleton>::Instance().loadFromSettings();

    qDebug() << "ProjectSingleton:: opening configuration for project " << fileName << ", qsettings " << mSettings->fileName();
    emit projectChanged();
    qDebug() << "Plot title: " << mSettings->value("Plot/Plot1/info/title", "error");
}

void ProjectSingleton::save()
{
    if(!mFileName.isEmpty())
    {
        // Save all curves
        Singleton<CurveSingleton>::Instance().save();
        mSettings->sync();
        Singleton<DataSingleton>::Instance().save();
        mSettings->sync();
        Singleton<FunctionsSingleton>::Instance().save();

        mSettings->sync();
        if(mSettings->status() != QSettings::NoError) {
            qDebug() << "Error while saving!";
            qDebug() << "XXX: do something with the error!";
        }
    }
}

/*!
 * Moves all values of current QSettings to a new QSettings synced with another path
 */
void ProjectSingleton::saveAs(const QString& newPath)
{
    QSettings *s = new QSettings(newPath, QSettings::IniFormat);

    if(s->allKeys().count() < 1)
    {
        QStringList keys = mSettings->allKeys();
        for(int i = 0; i < keys.count(); i++)
        {
            s->setValue(keys.at(i), mSettings->value(keys.at(i)));
        }
        delete mSettings;
        mSettings = s;
        save();
    } else {
        qDebug() << "WARNING: trying to move to non empty settings. Settings are being cleaned and then overwritten!";
        QStringList keys = mSettings->allKeys();
        for(int i = 0; i < keys.count(); i++)
        {
            s->setValue(keys.at(i), mSettings->value(keys.at(i)));
        }
        delete mSettings;
        mSettings = s;
        save();
    }
    emit projectChanged();
}

QSettings *ProjectSingleton::getSettings()
{
    return mSettings;
}
