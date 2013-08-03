#ifndef PLOTAREA_H
#define PLOTAREA_H

#include <QWidget>

class QAction;
class PlotControlWindow;
class PlotWidget;

namespace Ui {
class PlotArea;
}

class PlotArea : public QWidget
{
    Q_OBJECT

public:
    explicit PlotArea(const QString &name, const unsigned int id, QWidget *parent = 0);
    ~PlotArea();

    PlotWidget *getPlotWidget();

    void setId(unsigned int id);
    unsigned int getId() const;

private:
    Ui::PlotArea *ui;

    QString mName;
    PlotControlWindow *mConfigWindow;

public slots:
    void openConfigDialog();
    void plotConfigChanged();
};

#endif // PLOTAREA_H
