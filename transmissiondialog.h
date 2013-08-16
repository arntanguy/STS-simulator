#ifndef TRANSMISSIONDIALOG_H
#define TRANSMISSIONDIALOG_H

#include <QDialog>

namespace Ui {
class TransmissionDialog;
}

class TransmissionFunction;

class TransmissionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransmissionDialog(QWidget *parent = 0);
    ~TransmissionDialog();

public Q_SLOTS:
    void accept();
private:
    void init();

private:
    Ui::TransmissionDialog *ui;
    TransmissionFunction *mTransmission;
};

#endif // TRANSMISSIONDIALOG_H
