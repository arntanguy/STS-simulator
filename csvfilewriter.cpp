#include "csvfilewriter.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

CSVFileWriter::CSVFileWriter(const QString &delimiter)
{
    mDelimiter = delimiter;
    mFile = 0;
}

CSVFileWriter::~CSVFileWriter()
{
    if(mFile != 0) {
        mFile->close();
        delete mFile;
    }
}

bool CSVFileWriter::openFile(const QString& filePath)
{
    mFile = new QFile(filePath);
    return mFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
}


bool CSVFileWriter::writeLine(const QStringList& line)
{
    qDebug() << "writeLine: " <<line;
    if(mFile != 0) {
        QTextStream out(mFile);
        out << line.join(mDelimiter) << "\n";
        return true;
    } else {
        return false;
    }
}
