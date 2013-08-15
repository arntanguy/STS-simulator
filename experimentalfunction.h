#ifndef EXPERIMENTALFUNCTION_H
#define EXPERIMENTALFUNCTION_H

#include "plotdata.h"
#include <QString>
#include <QMetaType>

class QSettings;

typedef double (*FuncPtr)(double);

class ExperimentalFunction : public QObject
{
    Q_OBJECT
    public:
        ExperimentalFunction();
        void setName(const QString &name) {
            mName = name;
        }
        QString getName() const {
            return mName;
        }
        void setFunction(FuncPtr function) {
            mFunction = function;
        }
        FuncPtr getFunction() const
        {
            return mFunction;
        }

        bool hasData() const {
            return mDataAvailable;
        }

        bool setData(const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName);

        double interpolate(double x);

        void loadFromSettings(QSettings *settings);
        void save(QSettings *settings);

    private:
        QString mName;
        // Address of function
        FuncPtr mFunction;

    public:
        bool mDataAvailable;
        PlotData mData;

    QString mExperimentalAbscissia, mExperimentalOrdinate;
    QString mExperimentalFiles;
};

Q_DECLARE_METATYPE(ExperimentalFunction *);

#endif // EXPERIMENTALFUNCTION_H
