#ifndef PLOTDATA_H
#define PLOTDATA_H

#include <QVector>

struct PlotData
{
    QVector<double> x;
    QVector<double> y;
    PlotData() {
    }
    PlotData(int resolution) {
        x.reserve(resolution);
        y.reserve(resolution);
    }

    void clear() {
        x.clear();
        y.clear();
    }

    void reserve(int n) {
        x.reserve(n);
        y.reserve(n);
    }

    int size() const {
        return x.size();
    }
};


#endif // PLOTDATA_H
