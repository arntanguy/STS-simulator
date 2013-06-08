#ifndef PLOTAREA_H
#define PLOTAREA_H

#include <QWidget>

class QAction;
class PlotControlDialog;

namespace Ui {
class PlotArea;
}

class PlotArea : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlotArea(const QString &name, QWidget *parent = 0);
    ~PlotArea();
    
private:
    Ui::PlotArea *ui;

    QString mName;
    PlotControlDialog *mConfigDialog;

public slots:
    void openConfigDialog();
    void plotConfigChanged();
};

#endif // PLOTAREA_H
