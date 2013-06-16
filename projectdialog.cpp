#include "projectdialog.h"
#include "ui_projectdialog.h"

#include <QSettings>
#include <QDebug>

ProjectDialog::ProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectDialog)
{
    ui->setupUi(this);

    initFromConfig();

    connect(ui->newProjectButton, SIGNAL(clicked()), parent, SLOT(slotNewProject()));
    connect(ui->newProjectButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(ui->openProjectButton, SIGNAL(clicked()), this, SLOT(slotOpenProject()));
    connect(ui->openProjectButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(this, SIGNAL(openProject(QString&)), parent, SLOT(slotOpenProject(QString&)));
}

ProjectDialog::~ProjectDialog()
{
    delete ui;
}


void ProjectDialog::initFromConfig()
{
    QSettings settings;
    settings.beginGroup("project");
    QString strList = settings.value("recentList","").toString();
    QStringList list = strList.split(",", QString::SkipEmptyParts);
    ui->recentProjectListWidget->addItems(list);
    settings.endGroup();
}



// ======================== SLOTS ============================
void ProjectDialog::slotOpenProject()
{
    QString path = ui->recentProjectListWidget->currentItem()->text();
    if(!path.isEmpty()) {
        qDebug() << "ProjectDialog::slotOpenProject(): emitting signal openProject("<<path<<")";
        emit openProject(path);
    }
}
