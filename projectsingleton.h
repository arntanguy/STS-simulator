#ifndef PROJECTSINGLETON_H
#define PROJECTSINGLETON_H

#include <QString>
#include <QSettings>
#include "singleton.h"

class ProjectSingleton
{
public:
    ProjectSingleton();
    ~ProjectSingleton();
    void createNewProject(const QString &fileName);
    void openProject(const QString& fileName);
    void saveAs(const QString& newPath);
    void save();

    QSettings *getSettings();
    bool hasProject() const {
        return !mFileName.isEmpty();
    }

private:
    QString mFileName;
    QSettings *mSettings;
};

#endif // PROJECTSINGLETON_H
