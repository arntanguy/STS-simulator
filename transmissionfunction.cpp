#include "transmissionfunction.h"
#include "projectsingleton.h"
#include "singleton.h"
#include "muParser.h"

double TransmissionFunction::phi = 0;
double TransmissionFunction::Z0 = 0;

TransmissionFunction::TransmissionFunction(QObject *parent) : Function(parent)
{
}

double TransmissionFunction::compute(const QString& dzName, double V, double e)
{
    QString expression = "exp(-_alpha * (Z0+"+dzName+"(V)) * sqrt(phi + V/2 - e) )";
    Function::setExpression(expression);
    mParser->Eval();

    setVariable("phi", phi);
    setVariable("Z0", Z0);
    setVariable("V", V);
    setVariable("e", e);

    return mParser->Eval();
}

void TransmissionFunction::loadFromSettings()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Transmission");
    phi = settings->value("phi", phi).toDouble();
    Z0 = settings->value("Z0", Z0).toDouble();
    settings->endGroup();
}

void TransmissionFunction::save()
{
    QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
    settings->beginGroup("Transmission");
    settings->setValue("phi", phi);
    settings->setValue("Z0", Z0);
    settings->endGroup();
}
