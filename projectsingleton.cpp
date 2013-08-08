#include "projectsingleton.h"
#include "curvesingleton.h"
#include "datasingleton.h"
#include "functionssingleton.h"
#include "plotsingleton.h"
#include "globalsettingssingleton.h"
#include "globalconstants.h"
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
    qDebug() << "ProjectSingleton::createNewProject() - "<<fileName;
    if(!mFileName.isEmpty()) {
        // XXX
        qDebug() << "ProjectSingleton::createNewProject() - filename isn't empty, a project is open, manage it!";
        qDebug() << "XXX: saving by default";
        save();
    }

    // First clear the old project
    clearAll();

    mFileName = fileName;
    delete mSettings;
    mSettings = new QSettings(fileName, QSettings::IniFormat);
    qDebug() << "Creating new configuration for project " << fileName;
    initFromDefaultProject();
    openProjectFromSettings();
}

void ProjectSingleton::initFromDefaultProject()
{
    qDebug() << "ProjectSingleton::initFromDefaultProject()";
    mSettings->clear();

    mFileName = DEFAULT_PROJECT;
    QSettings defaultProject(DEFAULT_PROJECT, QSettings::IniFormat);
    const QStringList keys = defaultProject.allKeys();
    Q_FOREACH(QString key, keys) {
        qDebug() << "Copying " << key << "\t" << defaultProject.value(key);
        mSettings->setValue(key, defaultProject.value(key));
    }
}

void ProjectSingleton::clearAll()
{
    qDebug() << "\n\n";
    qDebug() << "Clearing all";
    Singleton<DataSingleton>::Instance().clear();
    Singleton<CurveSingleton>::Instance().clear();
    Singleton<FunctionsSingleton>::Instance().clear();
    qDebug() << "\n\n";
}

void ProjectSingleton::openProjectFromSettings()
{
    qDebug() << "ProjectSingleton::openProjectFromSettings()";
    Singleton<GlobalSettingsSingleton>::Instance().loadFromSettings();
    // Load all curves
    qDebug() << "\n\n";
    Singleton<PlotSingleton>::Instance().loadFromSettings();
    qDebug() << "\n\n";
    Singleton<DataSingleton>::Instance().loadFromSettings();
    qDebug() << "\n\n";
    Singleton<CurveSingleton>::Instance().loadFromSettings();
    qDebug() << "\n\n";
    Singleton<FunctionsSingleton>::Instance().loadFromSettings();
    qDebug() << "\n\n";

    emit projectChanged();
}

void ProjectSingleton::openProject(const QString& fileName)
{
    if(!mFileName.isEmpty()) {
        // XXX
        qDebug() << "filename isn't empty, a project is open, manage it!";
        qDebug() << "XXX: saving by default";
        save();
    }

    qDebug() << "ProjectSingleton:: opening configuration for project " << fileName << ", qsettings " << mSettings->fileName();

    clearAll();

    mFileName = fileName;
    delete mSettings;
    mSettings = new QSettings(fileName, QSettings::IniFormat);

    openProjectFromSettings();
}

void ProjectSingleton::save()
{
    if(!mFileName.isEmpty())
    {
        Singleton<GlobalSettingsSingleton>::Instance().save();
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



