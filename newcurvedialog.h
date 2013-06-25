#ifndef NEWCURVEDIALOG_H
#define NEWCURVEDIALOG_H

#include <QDialog>

class Curve;

namespace Ui {
class NewCurveDialog;
}

class NewCurveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCurveDialog(QWidget *parent = 0);
    ~NewCurveDialog();

    void loadFromCurve(Curve *curve);

private:
    void init();

private:
    Ui::NewCurveDialog *ui;
    Curve *mCurve;

public slots:
    void accept();
    void loadDataFile();
    void dataFileChanged(int);
    void curveTypeChanged(int);
    void newFunction();
    void selectFunction();
};

#endif // NEWCURVEDIALOG_H
