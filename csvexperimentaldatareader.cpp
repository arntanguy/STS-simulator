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
        mData.append(DataColumn<double>());
    }
}

void CSVExperimentalDataReader::processLine(const QString &line)
{
    QStringList split = splitLine(line);
    qDebug() << split;

    for(int i=0; i<mColumnTitles.size(); i++) {
        if(i < split.size()) {
            double value = split[i].toDouble();
            mData[i].addValue(value);
        }
    }
}


//XXX: precision is ignored for now
DataColumn<double> CSVExperimentalDataReader::getColumn(int column, int precision)
{
    if(column < mData.size()) {
        // XXX take care of precision
        return mData[column];
    }
}

