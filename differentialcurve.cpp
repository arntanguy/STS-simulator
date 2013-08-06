#include "differentialcurve.h"
#include "differentialfunction.h"
#include "plotwidget.h"

DifferentialCurve::DifferentialCurve() : FunctionCurve()
{
    init();
}

DifferentialCurve::DifferentialCurve(int id) : FunctionCurve(id)
{
    init();
}

void DifferentialCurve::init()
{
    setType(Curve::Differential);
}

void DifferentialCurve::update(bool forceUpdate)
{
    qDebug() << "DifferentialCurve::update()";
    FunctionCurve::update(forceUpdate);
}


void DifferentialCurve::attach(PlotWidget *plot)
{
    FunctionCurve::attach(plot);
}

void DifferentialCurve::save()
{
    qDebug() << "DifferentialCurve::save()";
    FunctionCurve::save();
}

void DifferentialCurve::loadFromSettings()
{
    FunctionCurve::loadFromSettings();
}
/**
 * Differentiate
 **/

void DifferentialCurve::updateData()
{
    qDebug() << "=============++++++++++++++++=================";
    qDebug() << "DifferentialCurve::updateData()";
    qDebug() << "=============++++++++++++++++=================";
    if(mFunction != 0) {
        DifferentialFunction *f = dynamic_cast<DifferentialFunction *>(mFunction);
        if(f != 0) {
            PlotData data = f->differentiate();
           // qDebug() << "differential data";
            qDebug() << "V: " <<data.x;
            qDebug() << "f'(V): " <<data.y;
            mXData = data.x;
            mYData = data.y;
        } else {
            qDebug() << "DifferentialCurve::ERROR - Unable to compute differential: invalid function type.";
        }
    }
    mNeedsUpdate = false;
}


// ==================================== PUBLIC ==============================
void DifferentialCurve::setFunction(DifferentialFunction *f)
{
    qDebug() << "DifferentialCurve::setFunction() -- "<<f->getName();
    //XXX: handle this properly
    if(f != 0) {
        FunctionCurve::setFunction(f);
        qDebug() << "connecting function";
        connect(f, SIGNAL(integralDataComputed()), this, SLOT(slotUpdateData()));
        mFunction = f;
    } else {
        qDebug() << "DifferentialFunction::setFunction -- ERROR invalid function";
    }
}

// =================================== SLOTS ================================
void DifferentialCurve::slotUpdateData()
{
    qDebug() << "DifferentialCurve::slotUpdateData";
    update(true);
}
