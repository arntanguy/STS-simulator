#ifndef EXPERIMENdoubleALDAdoubleA_H
#define EXPERIMENdoubleALDAdoubleA_H

#include "data.h"

#include "datacolumn.h"
#include <QMap>
#include <QStringList>

class ExperimentalData : public Data
{
public:
    ExperimentalData();
    virtual ~ExperimentalData();

    void loadFromFile(const QString &file);

    virtual DataColumn<double> getColumn(const QString &column, int precision);

    QStringList getAvailableColumns() const {
        return mData.keys();
    }

    QString getId() const {
        return mId;
    }
private:
    QMap<QString, DataColumn<double>> mData;
    QString mId;
};

#endif // EXPERIMENdoubleALDAdoubleA_H
