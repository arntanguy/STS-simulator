#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class PlotArea;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void readSettings();

public slots:
     void actionAbout(bool);
    
private:
    Ui::MainWindow *ui;

    PlotArea *mPlotArea1;
    PlotArea *mPlotArea2;
    PlotArea *mPlotArea3;
    PlotArea *mPlotArea4;

};

#endif // MAINWINDOW_H
