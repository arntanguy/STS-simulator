#include "projectsingleton.h"
#include <QDebug>


ProjectSingleton::ProjectSingleton()
{
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
    }
}

void ProjectSingleton::openProject(const QString& fileName)
{
    if(!mFileName.isEmpty()) {
        // XXX
        qDebug() << "filename isn't empty, a project is open, manage it!";
    } else {
        mFileName = fileName;
        delete mSettings;
        mSettings = new QSettings(fileName, QSettings::IniFormat);
        qDebug() << "ProjectSingleton:: opening configuration for project " << fileName;
    }
}

QSettings *ProjectSingleton::getSettings()
{
    return mSettings;
}
