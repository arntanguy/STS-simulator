#include "experimentalfunction.h"
#include "datasingleton.h"
#include "data.h"
#include "typedefs.h"

#include <cmath>
#include <QDebug>

ExperimentalFunction::ExperimentalFunction()
{
    mDataAvailable = false;
}

bool ExperimentalFunction::setData(const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName)
{
    DataPtr data = Singleton<DataSingleton>::Instance().getData(experimentId);
    if(data != 0) {
        mExperimentalFiles = experimentId;
        mExperimentalAbscissia = abscissiaColumnName;
        mExperimentalOrdinate = ordinateColumnName;
        mDataAvailable = true;
        mData.x = data->getColumn(abscissiaColumnName).getData();
        mData.y = data->getColumn(ordinateColumnName).getData();
    } else {
        return false;
    }
    return true;
}

double ExperimentalFunction::interpolate(double x)
{
    // Assumes ordered X
    if(mData.size() > 2) {
        int i=0;

        while(x-mData.x[i] >= 0 && i<mData.size()) {
            i++;
        }
        qDebug() << "Match  i="<<i<<", xdata="<<mData.x[i] << " for real x="<<x;
        return mData.y[i];

        // It's an exact match
        if(x == mData.x[i]) {
            return mData.y[i];
        } else {
            double x1, x2, y1, y2;
            // Do interpolate
            if(i+1 < mData.size()) {
                x1 = mData.x[i];
                x2 = mData.x[i+1];
                y1 = mData.y[i];
                y2 = mData.y[i+1];
                return (y2+y1)/std::abs(x2-y1);
            } else {
                // Not enough point to interpolate, return closest point
                return mData.y[i];
            }
        }
    }
    return 0;
}
