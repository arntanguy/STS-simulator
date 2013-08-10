#ifndef VALUESELECTOR_H
#define VALUESELECTOR_H

#include <QWidget>
#include "typedefs.h"

namespace Ui {
class ValueSelector;
}
class QSettings;
class Function;

class ValueSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ValueSelector(const QString &name, const FunctionPtr &f, QWidget *parent = 0);
    ~ValueSelector();

    void save();
    void loadFromConfig();

public Q_SLOTS:
    void sliderRangeChanged(double, double);
    void variableValueChanged(double);
    void sliderValueChanged(double);

Q_SIGNALS:
    void valueChanged(double);
    void valueChanged(const QString &, double);

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
