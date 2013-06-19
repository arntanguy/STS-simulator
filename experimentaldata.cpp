#include "experimentaldata.h"
#include "csvexperimentaldatareader.h"


ExperimentalData::ExperimentalData()
{
}

ExperimentalData::~ExperimentalData()
{
}

void ExperimentalData::loadFromFile(const QString &file)
{
    CSVExperimentalDataReader reader;
    reader.parseFile(file, "\t");

    mData = reader.getData();
    mId = file;
}

DataColumn<double> ExperimentalData::getColumn(const QString &column, int precision)
{
    // XXX: take care of precision
    return mData[column];
}
