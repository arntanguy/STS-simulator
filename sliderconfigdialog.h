#ifndef SLIDERCONFIGDIALOG_H
#define SLIDERCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class SliderConfigDialog;
}

class Slider;

class SliderConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SliderConfigDialog(QWidget *parent = 0);
    ~SliderConfigDialog();

    double getMin() const;
    double getMax() const;
    double getStep() const;
    bool applyToAll() const;
    void initFromSlider(const Slider *s);

private:
    Ui::SliderConfigDialog *ui;
};

#endif // SLIDERCONFIGDIALOG_H
