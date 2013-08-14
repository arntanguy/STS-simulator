#ifndef PLOTSINGLETON_H
#define PLOTSINGLETON_H

#include <QMap>
#include <QList>
#include "singleton.h"

class PlotWidget;

class PlotSingleton
{
public:
    PlotSingleton();

    void addPlot(PlotWidget *plot);
    PlotWidget* getPlot(unsigned int id);

    void loadFromSettings();
    void save();

    QList<unsigned int> getPlotIds() const;

private:
    QMap<unsigned int, PlotWidget *> mPlots;
};

#endif // PLOTSINGLETON_H
