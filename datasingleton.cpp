#include "datasingleton.h"
#include "projectsingleton.h"
#include "experimentaldata.h"

#include <QDebug>
#include <QSettings>

DataSingleton::DataSingleton() : QObject()
{
};

void DataSingleton::save()
{
    qDebug() << "DataSingleton::save(): Saving all data...";
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();

    // First cleanup the curves
    settings->beginGroup("Data");
    settings->remove("");
    // Now save all the curves
    QStringList keys = mData.keys();
    QStringList paths;
    foreach(QString key, keys) {
        if(!key.isEmpty()) {
            paths << key;
        }
    }
    settings->setValue("paths", paths);
    settings->endGroup();
}

void DataSingleton::loadFromSettings()
{
    qDebug()<< "DataSingleton::loadFromSettings()";
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    // First cleanup the curves
    settings->beginGroup("Data");
    QStringList paths = settings->value("paths", QStringList()).toStringList();
    settings->endGroup();

    foreach(QString path, paths) {
        ExperimentalData *data = new ExperimentalData();
        data->loadFromFile(path);
        Singleton<DataSingleton>::Instance().addData(data);
    }
}

void DataSingleton::clear()
{
    foreach(QString key, mData.keys()) {
        mData[key]->deleteLater();
    }
    mData.clear();
}
