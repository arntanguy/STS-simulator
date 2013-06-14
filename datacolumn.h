#ifndef DATAROW_H
#define DATAROW_H

#include <QString>
#include <QVector>

template<typename T>
class DataColumn
{
public:
    DataColumn() {};
    DataColumn(const QString &title) { setTitle(title); };

public:
    void setTitle(const QString &title) { mTitle = title; }
    QString title() const { return mTitle; }

    void addValue(T value) {
        mData.append(value);
    }

    QVector<T> getData() {
        return mData;
    }
private:
    QString mTitle;
    QVector<T> mData;
};

#endif // DATAROW_H
