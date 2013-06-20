#include "experimentaldata.h"
#include "csvexperimentaldatareader.h"


ExperimentalData::ExperimentalData()
{
    mType = Experimental;
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

DataColumn<double> ExperimentalData::getColumn(const QString &column)
{
    return mData[column];
}
