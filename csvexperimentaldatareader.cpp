#include "csvexperimentaldatareader.h"

#include <QDebug>

CSVExperimentalDataReader::CSVExperimentalDataReader() : CSVFileReader()
{
}

CSVExperimentalDataReader::~CSVExperimentalDataReader()
{
}

/** ========================== VIRTUAL FUNCTIONS ========================================= **/
void CSVExperimentalDataReader::processTitleLine(const QString &line)
{
    CSVFileReader::processTitleLine(line);
    for(int i=0; i<mColumnTitles.size(); i++) {
        DataColumn<double> col(mColumnTitles[i]);
        mData[mColumnTitles[i]] = col;

    }
}

void CSVExperimentalDataReader::processLine(const QString &line)
{
    QStringList split = splitLine(line);
    //qDebug() << split;

    for(int i=0; i<mData.size(); i++) {
        if(i < split.size()) {
            double value = split[i].toDouble();
            mData[mColumnTitles[i]].addValue(value);
        }
    }
}


//XXX: precision is ignored for now
DataColumn<double> CSVExperimentalDataReader::getColumn(const QString& column, int precision)
{
    // XXX take care of precision
    return mData[column];
}

