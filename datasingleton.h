#ifndef DATASINGLETON_H
#define DATASINGLETON_H

#include <QObject>
#include <QMap>

#include "singleton.h"
#include "data.h"
#include <QSharedPointer>


class DataSingleton : public QObject
{
    Q_OBJECT
public:
    DataSingleton();

    void addData(Data *data)
    {
        QSharedPointer<Data> d(data);
        mData[data->getId()] = d;
        emit newDataAvailable(d);
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

    QSharedPointer<Data> getData(const QString& id) {
        return mData[id];
    }

    void loadFromSettings();
    void save();
    void clear();

signals:
    void newDataAvailable(QSharedPointer<Data>);

public slots:

private:
    QMap< QString, QSharedPointer<Data> >	mData;
};

#endif // DATASINGLETON_H
