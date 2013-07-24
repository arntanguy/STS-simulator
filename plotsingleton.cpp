#include "plotsingleton.h"

#include "plotwidget.h"

PlotSingleton::PlotSingleton()
{
}

void PlotSingleton::addPlot(PlotWidget *plot)
{
    if(plot != 0) mPlots[plot->getId()] = plot;
}

PlotWidget* PlotSingleton::getPlot(unsigned int id)
{
    return mPlots[id];
}

void PlotSingleton::loadFromSettings()
{
    qDebug()<< "PlotSingleton::loadFromSettings()";
    foreach(PlotWidget *plot, mPlots) {
        plot->loadFromSettings();
    }
}

void PlotSingleton::save()
{
    qDebug() << "Saving all plots (do nothing)...";
}
