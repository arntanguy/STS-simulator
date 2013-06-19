#ifndef DATASINGLETON_H
#define DATASINGLETON_H

#include <QObject>
#include <QMap>

#include "singleton.h"
#include "data.h"


class DataSingleton : public QObject
{
    Q_OBJECT
public:
    DataSingleton();

    void addData(Data *data)
    {
        mData[data->getId()] = data;
    }

    Data* getData(const QString& id) {
        return mData[id];
    }

signals:
    
public slots:
    
private:
    QMap<QString, Data *>	mData;
};

#endif // DATASINGLETON_H
