#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "plotwidget.h"
#include "plotarea.h"
#include "globalconfigdialog.h"
#include "newfunctiondialog.h"

#include "globalsettingssingleton.h"
#include "projectsingleton.h"
#include "plotsingleton.h"


#include <QCloseEvent>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Creates the first instance of the singleton
    ProjectSingleton *singleton = &Singleton<ProjectSingleton>::Instance();
    singleton->loadDefaultConfig();
    Singleton<GlobalSettingsSingleton>::Instance().loadFromSettings();

    mPlotArea1 = new PlotArea("Plot1", 0);
    mPlotArea2 = new PlotArea("Plot2", 1);
    mPlotArea3 = new PlotArea("Plot3", 2);
    mPlotArea4 = new PlotArea("Plot4", 3);
    PlotSingleton *pSingleton = &Singleton<PlotSingleton>::Instance();
    pSingleton->addPlot(mPlotArea1->getPlotWidget());
    pSingleton->addPlot(mPlotArea2->getPlotWidget());
    pSingleton->addPlot(mPlotArea3->getPlotWidget());
    pSingleton->addPlot(mPlotArea4->getPlotWidget());
    pSingleton->loadFromSettings();

    QVBoxLayout *l1 = new QVBoxLayout;
    l1->addWidget(mPlotArea1);
    ui->widget1->setLayout(l1);

    QVBoxLayout *l2 = new QVBoxLayout;
    l2->addWidget(mPlotArea2);
    ui->widget2->setLayout(l2);

    QVBoxLayout *l3 = new QVBoxLayout;
    l3->addWidget(mPlotArea3);
    ui->widget3->setLayout(l3);

    QVBoxLayout *l4 = new QVBoxLayout;
    l4->addWidget(mPlotArea4);
    ui->widget4->setLayout(l4);

    // Show main window maximized
    showMaximized();

    // Connect Menu Events
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout(bool)));
    connect(ui->actionLoad_Experimental_Data, SIGNAL(triggered(bool)), this, SLOT(actionLoadExperimentalData(bool)));
    connect(ui->actionSave_As, SIGNAL(triggered(bool)), this, SLOT(actionSaveAs(bool)));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(actionSave(bool)));
    connect(ui->actionNew_Project, SIGNAL(triggered(bool)), this, SLOT(actionNewProject(bool)));
    connect(ui->actionLoad_Project, SIGNAL(triggered(bool)), this, SLOT(actionLoadProject(bool)));
    connect(ui->actionNew_Base_Function, SIGNAL(triggered(bool)), this, SLOT(newBaseFunction(bool)));

    connect(ui->actionGlobal_Settings, SIGNAL(triggered(bool)), this, SLOT(actionGlobalSettings(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Virtual
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Close event, save...";
    actionSave(true);
    event->accept();
}

// ============== PRIVATE FUNCTIONS ================
void MainWindow::openProject(const QString &project) {
    // 1 - Save current Project

    // 2 - Load default config
    // XXX: TODO

    // 3 - Load project config
    ProjectSingleton *singleton = &Singleton<ProjectSingleton>::Instance();
    // The singleton will emit the signal when the configuration is loaded
    singleton->openProject(project);
}


// =============== SLOTS ===================
void MainWindow::actionAbout(bool)
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::actionLoadExperimentalData(bool)
{
 //   // Load previously used directory when loading experimental data
 //   QString startDir = mSettings.value("Save/experimentalDataDirectory", "").toString();

 //   QString fileName = QFileDialog::getOpenFileName(this,
 //    tr("Open Experimental Data"), startDir, tr("Experimental Data Files(*.csv *.txt);;All Files (*.*)"));


 //   CSVExperimentalDataReader<double> reader;
 //   reader.parseFile(fileName, "\t");


 //   const char *colors[] =
 //   {
 //       "LightSalmon",
 //       "SteelBlue",
 //       "Yellow",
 //       "Fuchsia",
 //       "PaleGreen",
 //       "PaleTurquoise",
 //       "Cornsilk",
 //       "HotPink",
 //       "Peru",
 //       "Maroon"
 //   };
 //   const int numColors = sizeof( colors ) / sizeof( colors[0] );
 //   Curve *curve = new Curve( QString("Experimental DZ2") );
 //   curve->setPen( QColor( colors[ numColors ] ), 2 );
 //   curve->setSamples(reader.getColumn("V", 1000).getData(), reader.getColumn("DZ1", 1000).getData());
 //   curve->attach( mPlotArea3->getPlotWidget() );
 //   mPlotArea3->getPlotWidget()->replot();



 //   // Save currently used directory for later use
 //   if(!fileName.isNull()) {
 //       mSettings.setValue("Save/experimentalDataDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
 //   }
}

void MainWindow::actionLoadProject(bool)
{
    // Load previously used directory when loading experimental data
    QString startDir = mSettings.value("Save/projectDirectory", "").toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Project location"),
                                                    startDir,
                                                    tr("STS-Project (*.sts);;All Files (*.*)"));
    if(!fileName.isEmpty()) {
        qDebug() << "Loading Project : " << fileName;
        openProject(fileName);
        mSettings.setValue("Save/projectDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
    }
}

void MainWindow::actionSave(bool)
{
    ProjectSingleton *singleton = &Singleton<ProjectSingleton>::Instance();
    if(singleton->hasProject()) {
        singleton->save();
    } else {
        actionSaveAs(true);
    }
}

void MainWindow::actionSaveAs(bool)
{
    // Load previously used directory when loading experimental data
    QString startDir = mSettings.value("Save/projectDirectory", "").toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Project location"),
                                                    startDir,
                                                    tr("STS-Project (*.sts);;All Files (*.*)"));
    qDebug() << fileName;
    if(!(fileName.endsWith(".sts") || fileName.endsWith(".STS"))) {
        fileName = fileName + ".sts";
    }

    // Save currently used directory for later use
    if(!fileName.isNull()) {
        mSettings.setValue("Save/projectDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
    }

    ProjectSingleton *singleton = &Singleton<ProjectSingleton>::Instance();
    singleton->saveAs(fileName);
}

void MainWindow::actionNewProject(bool)
{
    slotNewProject();
}

void MainWindow::actionGlobalSettings(bool)
{
    GlobalConfigDialog dialog;
    dialog.exec();
}

void MainWindow::slotNewProject()
{
    // Load previously used directory when loading experimental data
    QString startDir = mSettings.value("Save/newProjectDirectory", "").toString();

    qDebug() << "MainWindow::slotNewProject()";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Project location"),
                                                    startDir,
                                                    tr("STS-Project (*.sts);;All Files (*.*)"));
    qDebug() << fileName;
    if(!(fileName.endsWith(".sts") || fileName.endsWith(".STS"))) {
        fileName = fileName + ".sts";
    }

    // Save currently used directory for later use
    if(!fileName.isNull()) {
        mSettings.setValue("Save/newProjectDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
    }

    ProjectSingleton *singleton = &Singleton<ProjectSingleton>::Instance();
    singleton->createNewProject(fileName);

    QSettings settings;
    settings.beginGroup("project");
    settings.setValue("recentList",settings.value("recentList", "").toString()+","+fileName);
    settings.endGroup();
}

void MainWindow::slotOpenProject(QString &fileName)
{
    qDebug() << "MainWindow::slotOpenProject("<<fileName<<")";
    openProject(fileName);
}

void MainWindow::newBaseFunction(bool)
{
    NewFunctionDialog dialog(this);
    dialog.exec();
}
