#ifndef CSVFILEREADER_H
#define CSVFILEREADER_H

#include <QStringList>

class CSVFileReader
{
public:
    CSVFileReader();
    virtual void parseFile(const QString &fileName, const QString &delimiter);

protected:
    virtual void processTitleLine(const QString &line);
    virtual void processLine(const QString &line) = 0;
    QStringList splitLine(const QString &line);

protected:
    QStringList mColumnTitles;

private:
    QString mDelimiter;
};

#endif // CSVFILEREADER_H
