#ifndef PROJECTSINGLETON_H
#define PROJECTSINGLETON_H

#include <QString>
#include <QSettings>
#include "singleton.h"

class ProjectSingleton
{
public:
    ProjectSingleton();
    void createNewProject(const QString &fileName);
    void openProject(const QString& fileName);
    QSettings *getSettings();

private:
    QString mFileName;
    QSettings *mSettings;
};

#endif // PROJECTSINGLETON_H
