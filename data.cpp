#include "data.h"
#include "projectsingleton.h"

#include <QDebug>
#include <QSettings>

Data::Data()
{
}

void Data::save()
{
    qDebug() << "Saving data "<<getId();
    //QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
}
