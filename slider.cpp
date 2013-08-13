#include "slider.h"

#include <QMouseEvent>

Slider::Slider(QWidget *parent):
    QwtSlider(parent)
{
}

void Slider::setLowerBound(double low, double value)
{
    if(low >= value) {
        QwtSlider::setLowerBound(value);
    } else {
        QwtSlider::setLowerBound(low);
    }
}

void Slider::setUpperBound(double up, double value)
{
    if(value <= up) {
        QwtSlider::setUpperBound(up);
    } else {
        QwtSlider::setUpperBound(value);
    }
}

void Slider::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton) {
        emit rightClicked(this);
        return;
    } else {
        QwtSlider::mouseReleaseEvent(e);
    }
}
