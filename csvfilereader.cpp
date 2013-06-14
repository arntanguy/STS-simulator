#include "csvfilereader.h"

#include <QFile>
#include <QDebug>

CSVFileReader::CSVFileReader()
{
}

void CSVFileReader::parseFile(const QString &fileName, const QString &delimiter)
{
    qDebug() << "CSVFileReader::parseFile(" << fileName << ", " << delimiter << ")";
    mDelimiter = delimiter;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString line = file.readLine().replace("\n", "");
        if(line.isNull()) return;
        processTitleLine(line);

        while (!file.atEnd())
        {
            QString line = file.readLine().replace("\n", "");
            processLine(line);
        }
        file.close();
    }
}


void CSVFileReader::processTitleLine(const QString &line)
{
    mColumnTitles = splitLine(line);
    qDebug() << "CSVFileReader::processTitleLine -> " << mColumnTitles;
}

QStringList CSVFileReader::splitLine(const QString &line)
{
    return line.split(mDelimiter);
}
