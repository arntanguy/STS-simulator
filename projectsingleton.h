#ifndef PROJECTSINGLETON_H
#define PROJECTSINGLETON_H

#include <QString>
#include <QSettings>
#include <QObject>
#include "singleton.h"

/*!
 * Manages the loading and saving of the project and its preferences
 * WARNING: This singleton will call the slot projectChanged() everytime a new configuration is ready.
 * All widgets that are affected by the save should register with this signal to be updated of the new project.
 * Failure to do so would result in an incoherent state!
 */
class ProjectSingleton : public QObject
{

    Q_OBJECT

public:
    ProjectSingleton();
    ~ProjectSingleton();
    void loadDefaultConfig();
    void createNewProject(const QString &fileName);
    void openProject(const QString& fileName);
    void saveAs(const QString& newPath);
    void save();
    void clearAll();

    QSettings *getSettings();
    bool hasProject() const {
        return !mFileName.isEmpty();
    }

private:
    void initFromDefaultProject();
    void openProjectFromSettings();
    void resetFunctionStaticId();

private:
    QString mFileName;
    QSettings *mSettings;

signals:
    /*!
     * WARNING: This singleton will call the slot projectChanged() everytime a new configuration is ready.
     * All widgets that are affected by the save should register with this signal to be updated of the new project.
     * Failure to do so would result in an incoherent state!
     */
    void projectChanged();
};

#endif // PROJECTSINGLETON_H
