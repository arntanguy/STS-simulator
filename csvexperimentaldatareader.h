#ifndef CSVEXPERIMENTALDATAREADER_H
#define CSVEXPERIMENTALDATAREADER_H

#include "csvfilereader.h"
#include "datacolumn.h"
#include <QVector>


class CSVExperimentalDataReader : public CSVFileReader
{
public:
    CSVExperimentalDataReader();
    virtual ~CSVExperimentalDataReader();

    DataColumn<double> getColumn(int column, int precision);

protected:
    // ============= Virtual functions ===================
    virtual void processTitleLine(const QString &line);

    // ============= Pure virtual functions ===============
    virtual void processLine(const QString &line);

private:
    QVector<DataColumn<double>> mData;
};

#endif // CSVEXPERIMENTALDATAREADER_H
