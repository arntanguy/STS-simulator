#include "experimentaldata.h"
#include "csvexperimentaldatareader.h"

#include <QDebug>


ExperimentalData::ExperimentalData()
{
    mType = Experimental;
}

ExperimentalData::~ExperimentalData()
{
    qDebug() << "ExperimentalData::~ExperimentalData() - Removing all data";
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
