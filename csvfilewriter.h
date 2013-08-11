#ifndef CSVFILEWRITER_H
#define CSVFILEWRITER_H

#include <QString>
#include <QStringList>
#include <QFile>

class CSVFileWriter
{
public:
    CSVFileWriter(const QString &delimiter="\t");
    ~CSVFileWriter();

    bool openFile(const QString& filePath);
    bool writeLine(const QStringList& );

private:
    QString mDelimiter;
    QFile *mFile;
};

#endif // CSVFILEWRITER_H
