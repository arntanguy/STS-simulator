#ifndef TRANSMISSIONFUNCTION_H
#define TRANSMISSIONFUNCTION_H

#include "function.h"

#define TRANSMISSION_ALPHA 1

class TransmissionFunction : public Function
{
public:
    TransmissionFunction(QObject *parent = 0);

    double compute(const QString& dzName, double V, double e);

    static void setPhi(double value);
    static void setZ0(double value);

    void loadFromSettings();
    void save();
private:
    static double phi;
    static double Z0;

};

#endif // TRANSMISSIONFUNCTION_H
