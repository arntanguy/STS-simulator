#ifndef VALUESELECTOR_H
#define VALUESELECTOR_H

#include <QWidget>

namespace Ui {
class ValueSelector;
}
class QSettings;
class Function;

class ValueSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ValueSelector(const QString &name, Function *f, QWidget *parent = 0);
    ~ValueSelector();

    void save();
    void loadFromConfig();

public Q_SLOTS:
    void minValueChanged(double);
    void maxValueChanged(double);
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
    Function *mFunction;
    double *mVariable;
    QString mName;
};

#endif // VALUESELECTOR_H
