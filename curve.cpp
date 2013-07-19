#include "curve.h"
#include <algorithm>
#include "projectsingleton.h"
#include "datasingleton.h"
#include "plotwidget.h"

unsigned int Curve::mCurveStaticId = 0;

Curve::Curve()
{
    init();
    mResolution = 1;
    setId(mCurveStaticId);
}

Curve::Curve(int id)
{
    setId(id);
}

Curve::Curve(const QString &name)
{
    init();
    mResolution = 1;
    setTitle(name);
    setId(mCurveStaticId);
}

void Curve::copyFromCurve(Curve *curve)
{
    qDebug() << "Curve::copyFromCurve() - name " << curve->title().text();
    setExperimentalData(mExperimentalId, mExperimentalAbscissia, mExperimentalOrdinate);
    setTitle(curve->title());
    setPen(curve->pen());
}

void Curve::init()
{
    setRenderHint( QwtPlotItem::RenderAntialiased );
}

void Curve::setId(unsigned int id)
{
    qDebug() << "set id " << id;
    if(id >= mCurveStaticId) {
        mCurveId = id;
        mCurveStaticId = std::max(id, mCurveStaticId) + 1;
    } else {
        // XXX: handle error
        mCurveId = -1;
    }
}

void Curve::setExperimentalData(const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName)
{
    mData = Singleton<DataSingleton>::Instance().getData(experimentId);
    mExperimentalId = experimentId;
    mExperimentalAbscissia = abscissiaColumnName;
    mExperimentalOrdinate = ordinateColumnName;
    if(mData != 0) {
        setSamples(mData->getColumn(abscissiaColumnName).getData(), mData->getColumn(ordinateColumnName).getData());
        foreach(Curve *curve, mPlots) {
            curve->setExperimentalData(experimentId, abscissiaColumnName, ordinateColumnName);
        }
    }
}

/*!
 * \brief Curve::setResolution
 *  Sets the number of points to be computed for the curve
 *  Careful, changing this value could result in all the points being recomputed,
 *  which could be quite an expensive operation!
 * \param resolution
 */
void Curve::setResolution(int resolution)
{
    mResolution = resolution;
}

/*!
 * \brief Curve::loadFromSettings
 *  Load curve from settings.
 *  WARNING: the curve id must be set beforehand Curve *curve = new Curve(42);
 *  Project settings state must be at root group
 */
void Curve::loadFromSettings()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(mCurveId));
    setTitle(settings->value("title", "Unknown").toString());
    setResolution(settings->value("resolution", 0).toInt());

    // Load data
    QString path = settings->value("data", "").toString();;
    Data::Type type = static_cast<Data::Type>(settings->value("type", Data::Experimental).toInt());
    if(type == Data::Experimental) {
        QString abscissia = settings->value("abscissia", "").toString();
        QString ordinate = settings->value("ordinate", "").toString();
        setExperimentalData(path, abscissia, ordinate);
    }

    // Load pen
    QPen pen;
    pen.setColor(settings->value("color", Qt::black).value<QColor>());
    pen.setWidth(settings->value("thickness", 0.0d).toDouble());
    pen.setStyle(static_cast<Qt::PenStyle>(settings->value("style", Qt::SolidLine).toInt()));
    // Apply pen
    setPen(pen);

    settings->endGroup();
}

void Curve::save()
{
    qDebug() << "Saving curve "<<mCurveId<<","<<title().text();
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(mCurveId)+"/");
    qDebug() << "current group: " << settings->group();
    settings->setValue("id", mCurveId);
    settings->setValue("title", title().text());
    settings->setValue("resolution", mResolution);

    settings->setValue("color", pen().color());
    settings->setValue("thickness", pen().width());
    settings->setValue("style", pen().style());
    if(mData != 0) {
        Data::Type type = mData->getType();
        settings->setValue("type", type);
        if(type == Data::Experimental) {
            settings->setValue("abscissia", mExperimentalAbscissia);
            settings->setValue("ordinate", mExperimentalOrdinate);
        }
        settings->setValue("data", mData->getId());
    }
    settings->endGroup();
}


// =============================== VIRTUAL ======================================
void Curve::update()
{
    //if(needsUpdate()) {
    //}
}

// =============================== SURDEFINE ====================================
void Curve::attach(PlotWidget *plot)
{
    qDebug() << "Cuve::attach() - attach curve " << title().text() << " to plot " << plot->getName();
    Curve *curve = 0;
    if(mPlots.contains(plot)) {
        qDebug() << "Curve already attached, update copies";
        curve = mPlots[plot];
        if(curve != 0) {
            curve->copyFromCurve(this);
        }
    } else {
        curve = new Curve(getId());
        curve->copyFromCurve(this);
        curve->QwtPlotItem::attach(plot);
        curve->setExperimentalData(mExperimentalId, mExperimentalAbscissia, mExperimentalOrdinate);
    }
    mPlots[plot] = curve;
}

void Curve::detach(PlotWidget *plot)
{
    Curve *curve = mPlots[plot];
    if(curve != 0) {
        curve->QwtPlotItem::detach();
        delete curve;
        mPlots.remove(plot);
    }
}

