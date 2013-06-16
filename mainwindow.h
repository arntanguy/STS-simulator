#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

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
    void openProject(const QString &project);

public slots:
    void actionAbout(bool);
    void actionLoadProject(bool);
    void actionLoadExperimentalData(bool);
    void actionSaveAs(bool);
    void actionSave(bool);
    void slotNewProject();
    void slotOpenProject(QString &fileName);

private:
    Ui::MainWindow *ui;

    PlotArea *mPlotArea1;
    PlotArea *mPlotArea2;
    PlotArea *mPlotArea3;
    PlotArea *mPlotArea4;

    QSettings mSettings;

};

#endif // MAINWINDOW_H
