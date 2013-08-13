#ifndef VALUESELECTOR_H
#define VALUESELECTOR_H

#include <QWidget>
#include "typedefs.h"

namespace Ui {
class ValueSelector;
}
class QSettings;
class Function;
class Slider;

class ValueSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ValueSelector(const QString &name, const FunctionPtr &f, QWidget *parent = 0);
    ~ValueSelector();

    void save();
    void loadFromConfig();

    void setRange(double, double, double);

public Q_SLOTS:
    void variableValueChanged(double);
    void sliderValueChanged(double);
    void configureSlider(const Slider *);
    void sliderReleased();
    void valueEntered();

Q_SIGNALS:
    void valueChanged(double);
    void valueChanged(const QString &, double);
    void configureAllSliders(double, double, double);

private:
    Ui::ValueSelector *ui;
    /**
     * Any change made to this variable will be repercuted when computing the function
     **/
    FunctionPtr mFunction;
    double *mVariable;
    QString mName;
};

#endif // VALUESELECTOR_H
