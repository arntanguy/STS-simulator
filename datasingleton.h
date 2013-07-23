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
        emit newDataAvailable(data);
    }

    QStringList getExperimentalDataPaths() {
        QStringList paths;
        foreach(QString key, mData.keys()) {
            if(mData[key] != 0) {
                if(mData[key]->getType() == Data::Experimental) {
                    paths << mData[key]->getId();
                }
            }
        }
        return paths;
    }

    Data* getData(const QString& id) {
        return mData[id];
    }

    void loadFromSettings();
    void save();

signals:
    void newDataAvailable(Data *);

public slots:

private:
    QMap<QString, Data *>	mData;
};

#endif // DATASINGLETON_H
