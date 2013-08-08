#ifndef CSVEXPERIMENTALDATAREADER_H
#define CSVEXPERIMENTALDATAREADER_H

#include "csvfilereader.h"
#include "datacolumn.h"
#include <QMap>

class CSVExperimentalDataReader : public CSVFileReader
{
public:
    CSVExperimentalDataReader();
    virtual ~CSVExperimentalDataReader();

    DataColumn<double> getColumn(const QString &column);
    QMap<QString, DataColumn<double>> getData() const {
                                 return mData;
};

protected:
    // ============= Virtual functions ===================
    virtual void processTitleLine(const QString &line);

    // ============= Pure virtual functions ===============
    virtual void processLine(const QString &line);

private:
    QMap<QString, DataColumn<double>> mData;
};

#endif // CSVEXPERIMENTALDATAREADER_H
