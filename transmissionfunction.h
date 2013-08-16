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
    static double getPhi() {
        return phi;
    }
    static double getZ0() {
        return Z0;
    }

    void loadFromSettings();
    void save();

    bool updateNeeded() const {
        return mUpdateNeeded;
    }
private:
    // In common between all transmission
    static double phi;
    static double Z0;

    static bool mUpdateNeeded;
};

#endif // TRANSMISSIONFUNCTION_H
