#ifndef SLIDER_H
#define SLIDER_H

#include <qwt_slider.h>

class QMouseEvent;

class Slider : public QwtSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);

    void setUpperBound(double, double);
    void setLowerBound(double, double);
signals:
    void rightClicked(const Slider *);

private slots:
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // SLIDER_H
