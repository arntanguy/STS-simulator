#ifndef DATA_H
#define DATA_H

#include <QObject>

#include "datacolumn.h"
#include <QMap>
#include <QStringList>

class Data : public QObject
{
    Q_OBJECT
public:
    enum Type {Experimental, Function};
    Data();
    virtual ~Data();

    virtual void setData(const QMap< QString, DataColumn<double> > &) {
    }
    virtual QStringList getAvailableColumns() const = 0;
    virtual DataColumn<double> getColumn(const QString &column) = 0;
    virtual QString getId() const = 0;

    virtual void save();

    Type getType() const {
        return mType;
    }

protected:
    Type mType;
};

#endif // DATA_H
