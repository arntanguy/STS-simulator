#include "curve.h"
#include <algorithm>
#include "projectsingleton.h"
#include "datasingleton.h"
#include "plotsingleton.h"
#include "plotwidget.h"
#include "globalsettingssingleton.h"

int Curve::mCurveStaticId = 0;

Curve::Curve()
{
    init();
    setId(mCurveStaticId);
}

Curve::Curve(int id)
{
    init();
    setId(id);
}

Curve::Curve(const QString &name)
{
    init();
    setTitle(name);
    setId(mCurveStaticId);
}

Curve::~Curve()
{
    qDebug() << "Curve::~Curve - Destroying curve, name=" << title().text();
    detachFromAll();
}

void Curve::abstractCopyFromCurve(Curve *curve)
{
    qDebug() << "Curve::abstractCopyFromCurve() - name " << curve->title().text();
    setTitle(curve->title().text());
    setMinMax(curve->getMin(), curve->getMax());
    setPen(curve->pen());
    mType = curve->getType();
}

void Curve::copyFromCurve(Curve *curve)
{
    qDebug() << "Curve::copyFromCurve() - name " << curve->title().text();
    abstractCopyFromCurve(curve);
    if(curve->getType() == Experimental) {
        setExperimentalData(mExperimentalId, mExperimentalAbscissia, mExperimentalOrdinate);
    }
}

void Curve::init()
{
    mType = Unknown;
    setRenderHint( QwtPlotItem::RenderAntialiased );
    setMinMax(0, 1);
    initGlobalSettings();
    GlobalSettingsSingleton *singleton = &Singleton<GlobalSettingsSingleton>::Instance();
    connect(singleton, SIGNAL(curveSettingsUpdated()), this, SLOT(initGlobalSettings()));
}

void Curve::initGlobalSettings()
{
    qDebug() << "=========== Curve::INIT GLOBAL SETTINGS ==============";
    GlobalSettingsSingleton *singleton = &Singleton<GlobalSettingsSingleton>::Instance();
    setMinMax(singleton->getMin(), singleton->getMax());
    updateExperimentalData();
    //setResolution(singleton->getResolution());
}

void Curve::setId(int id)
{
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
    qDebug() << "setting experimental data with min "<<mMin<<" max "<<mMax;
    mData = Singleton<DataSingleton>::Instance().getData(experimentId);
    mExperimentalId = experimentId;
    mExperimentalAbscissia = abscissiaColumnName;
    mExperimentalOrdinate = ordinateColumnName;
    if(mData != 0) {
        QVector<double> xVal, yVal;
        QVector<double> fullX = mData->getColumn(abscissiaColumnName).getData();
        QVector<double> fullY = mData->getColumn(ordinateColumnName).getData();
        for (int i = 0; i < fullX.size(); i++) {
            double x = fullX[i];
            if(x >= mMin && x <= mMax) {
                xVal.append(x);
                yVal.append(fullY[i]);
            }
        }
        //qDebug() << "data: "<<xVal;
        setSamples(xVal, yVal);
        foreach(PlotWidget *p, mPlots.keys()) {
            Curve *c=mPlots[p];
            if(c != 0) {
                c->setExperimentalData(experimentId, abscissiaColumnName, ordinateColumnName);
            } else {
                qDebug() << "Curve::setExperimentalData() - NULL CURVE";
            }
        }
    }
}

/**
 * Call updateExperimentalData() to make this new resolution effective
 **/
void Curve::setMinMax(double min, double max)
{
    if(min != mMin || max != mMax) {
        mMin = min;
        mMax = max;
        mNeedsUpdate = true;
        foreach(Curve *curve, mPlots) {
            if(curve != 0) {
                curve->setMinMax(min, max);
            } else {
                qDebug() << "Curve::setMinMax() - NULL CURVE";
            }
        }
    }

}

void Curve::updateExperimentalData()
{
    setExperimentalData(mExperimentalId, mExperimentalAbscissia, mExperimentalOrdinate);
    foreach(PlotWidget *p, mPlots.keys()) {
        Curve *curve = mPlots[p];
        if(curve != 0) {
            curve->setExperimentalData(mExperimentalId, mExperimentalAbscissia, mExperimentalOrdinate);
            p->replot();
            qDebug() << "update range";
            p->updateRange();
            p->replot();
        } else {
            qDebug() << "Curve::updateExperimentalData() - NULL CURVE";
        }
    }
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
    setMinMax(settings->value("min", 0).toDouble(), settings->value("max", 10).toDouble());

    Curve::Type type = static_cast<Curve::Type>(settings->value("type", Curve::Experimental).toUInt());
    setType(type);
    if(type == Curve::Experimental) {
        // Load data
        QString path = settings->value("data", "").toString();;
        QString abscissia = settings->value("abscissia", "").toString();
        QString ordinate = settings->value("ordinate", "").toString();
        setExperimentalData(path, abscissia, ordinate);
        updateExperimentalData();
    }

    // Load pen
    QPen pen;
    pen.setColor(settings->value("color", Qt::black).value<QColor>());
    pen.setWidth(settings->value("thickness", 0).toDouble());
    pen.setStyle(static_cast<Qt::PenStyle>(settings->value("style", Qt::SolidLine).toInt()));
    // Apply pen
    setPen(pen);

    QStringList attachedToPlots = settings->value("attachedToPlots", QStringList()).toStringList();
    foreach(QString plotId, attachedToPlots) {
        this->attach(Singleton<PlotSingleton>::Instance().getPlot(plotId.toUInt()));
    }
    settings->endGroup();
}

void Curve::save()
{
    qDebug() << "Saving curve "<<mCurveId<<","<<title().text();
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Curves/"+QString::number(mCurveId)+"/");
    settings->setValue("id", mCurveId);
    settings->setValue("title", title().text());

    settings->setValue("color", pen().color());
    settings->setValue("thickness", pen().width());
    settings->setValue("style", pen().style());
    settings->setValue("type", mType);
    settings->setValue("min", mMin);
    settings->setValue("max", mMax);
    if(mType == Curve::Experimental) {
        if(mData != 0) {
            settings->setValue("abscissia", mExperimentalAbscissia);
            settings->setValue("ordinate", mExperimentalOrdinate);
            settings->setValue("data", mData->getId());
        }
    }

    // Save on which plot the curve is attached
    QStringList plots;
    foreach(PlotWidget *plot, mPlots.keys()) {
        if(isAttached(plot)) plots << QString::number(plot->getId());
    }
    settings->setValue("attachedToPlots", plots);

    settings->endGroup();
}


void Curve::setTitle(const QString& title)
{
    QwtPlotItem::setTitle(title);
    foreach(Curve *curve, mPlots) {
        if(curve != 0)
            curve->setTitle(title);
    }
}
void Curve::setPen(const QPen& pen)
{
    QwtPlotCurve::setPen(pen);
    foreach(Curve *curve, mPlots) {
        if(curve != 0) {
            curve->setPen(pen);
        }
    }
    update();
}
// =============================== VIRTUAL ======================================
void Curve::update(bool forceUpdate)
{
    foreach(PlotWidget *plot, mPlots.keys()) {
        plot->replot();
    }
}

// =============================== SURDEFINE ====================================
void Curve::attach(PlotWidget *plot)
{
    qDebug() << "Cuve::attach() - attach curve " << title().text() << " to plot " << plot->getName();
    Curve *curve = 0;
    if(isAttached(plot)) {
        qDebug() << "Curve already attached, update copies";
        curve = mPlots[plot];
        if(curve != 0) {
            curve->copyFromCurve(this);
        }
    } else {
        curve = new Curve(getId());
        curve->copyFromCurve(this);
        curve->QwtPlotItem::attach(plot);
        if(curve->getType() == Experimental)
            curve->setExperimentalData(mExperimentalId, mExperimentalAbscissia, mExperimentalOrdinate);
    }
    mPlots[plot] = curve;
    connect(this, SIGNAL(rangeUpdated()), plot, SLOT(updateRange()));
}

void Curve::detach(PlotWidget *plot)
{
    Curve *curve = mPlots[plot];
    if(curve != 0) {
        qDebug () << "Deleting curve " << title().text() << " attached to " << plot->getId();
        curve->QwtPlotItem::detach();
        mPlots.remove(plot);
        delete curve;
    }
}

void Curve::detachFromAll()
{
    foreach(PlotWidget *p, mPlots.keys()) {
        Curve *c = mPlots[p];
        if(c != 0) {
            c->QwtPlotItem::detach();
            delete c;
            mPlots.remove(p);
            p->replot();
        } else {
                qDebug() << "Curve::detachFromAll() - NULL CURVE";
            }

    }
}

bool Curve::isAttached(PlotWidget *plot) const
{
    return plot != 0 && mPlots.contains(plot) && mPlots[plot] != 0;
}

bool Curve::isAttached(unsigned int plotId) const
{
    return isAttached(Singleton<PlotSingleton>::Instance().getPlot(plotId));
}

bool Curve::isAttached() const
{
    foreach(unsigned int id, Singleton<PlotSingleton>::Instance().getPlotIds()) {
        if(isAttached(id)) return true;
    }
    return false;
}



void Curve::resetStaticId()
{
    mCurveStaticId = 0;
}
