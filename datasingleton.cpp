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
    settings->setValue("paths", keys);
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
        // XXX: take into account functions as well
        ExperimentalData *data = new ExperimentalData();
        data->loadFromFile(path);
        Singleton<DataSingleton>::Instance().addData(data);
    }
}
