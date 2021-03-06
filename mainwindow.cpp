#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "plotwidget.h"
#include "plotarea.h"
#include "globalconfigdialog.h"
#include "newfunctiondialog.h"
#include "plotexportdialog.h"
#include "experimentalfunctiondialog.h"

#include "globalsettingssingleton.h"
#include "projectsingleton.h"
#include "plotsingleton.h"
#include "globalconstants.h"
#include "functionssingleton.h"
#include "exportcurvesdialog.h"


#include <QCloseEvent>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    removeToolBar(ui->mainToolBar);

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
    connect(ui->actionSave_As, SIGNAL(triggered(bool)), this, SLOT(actionSaveAs(bool)));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(actionSave(bool)));
    connect(ui->actionNew_Project, SIGNAL(triggered(bool)), this, SLOT(actionNewProject(bool)));
    connect(ui->actionLoad_Project, SIGNAL(triggered(bool)), this, SLOT(actionLoadProject(bool)));
    connect(ui->actionNew_Base_Function, SIGNAL(triggered(bool)), this, SLOT(newBaseFunction(bool)));
    connect(ui->actionEdit_Default_Project_Template, SIGNAL(triggered(bool)), this, SLOT(actionEditDefaultProjectTemplate(bool)));
    connect(ui->actionExport_Variables, SIGNAL(triggered(bool)), this, SLOT(actionExportVariables(bool)));
    connect(ui->actionExport_Plots, SIGNAL(triggered(bool)), this, SLOT(actionExportPlots(bool)));
    connect(ui->actionExport_Function_Data, SIGNAL(triggered(bool)), this, SLOT(actionExportFunctionData(bool)));
    connect(ui->actionDZ_Functions, SIGNAL(triggered(bool)), this, SLOT(actionDZFunctions(bool)));

    connect(ui->actionGlobal_Settings, SIGNAL(triggered(bool)), this, SLOT(actionGlobalSettings(bool)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout(bool)));
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
        ProjectSingleton *singleton = &Singleton<ProjectSingleton>::Instance();
        singleton->saveAs(fileName);
    }
}

void MainWindow::actionEditDefaultProjectTemplate(bool)
{
    qDebug() << "Loading Default Project : " << DEFAULT_PROJECT;
    openProject(DEFAULT_PROJECT);
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
    if(!fileName.isEmpty()) {
        if(!(fileName.endsWith(".sts") || fileName.endsWith(".STS"))) {
            fileName = fileName + ".sts";
        }

        // Save currently used directory for later use
        if(!fileName.isNull()) {
            mSettings.setValue("Save/newProjectDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
        }

        ProjectSingleton *singleton = &Singleton<ProjectSingleton>::Instance();
        singleton->createNewProject(fileName);
    }
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

void MainWindow::actionExportVariables(bool)
{
    QString startDir = mSettings.value("Save/variablesExport", "").toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export variables"),
                                                    startDir,
                                                    tr("All Files (*.*)"));

    if(!fileName.isEmpty()) {
        mSettings.setValue("Save/projectDirectory", QFileInfo(fileName).absoluteDir().absolutePath());

        Singleton<FunctionsSingleton>::Instance().exportVariables(fileName);
    }
}

void MainWindow::actionExportPlots(bool)
{
    PlotExportDialog dialog(this);
    dialog.addPlot(mPlotArea1->getPlotWidget());
    dialog.addPlot(mPlotArea2->getPlotWidget());
    dialog.addPlot(mPlotArea3->getPlotWidget());
    dialog.addPlot(mPlotArea4->getPlotWidget());
    dialog.exec();
}

void MainWindow::actionExportFunctionData(bool)
{
    ExportCurvesDialog dialog(this);
    dialog.exec();
}

void MainWindow::actionDZFunctions(bool)
{
    ExperimentalFunctionDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        //XXX: update functions?
    }
}
