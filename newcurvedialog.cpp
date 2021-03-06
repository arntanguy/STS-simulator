#include "newcurvedialog.h"
#include "ui_newcurvedialog.h"
#include "curve.h"
#include "curvesingleton.h"
#include "datasingleton.h"
#include "projectsingleton.h"
#include "experimentaldata.h"
#include "functionselectiondialog.h"
#include "newfunctiondialog.h"
#include "functioncurve.h"
#include "integralcurve.h"

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

NewCurveDialog::NewCurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCurveDialog)
{
    ui->setupUi(this);
    mCurve = 0;
    init();

    // Data page
    connect(ui->dataLoadButton, SIGNAL(clicked()), this, SLOT(loadDataFile()));
    connect(ui->dataLoaded, SIGNAL(currentIndexChanged(int)), this, SLOT(dataFileChanged(int)));

    mMayClose = true;
}

NewCurveDialog::~NewCurveDialog()
{
    delete ui;
}

void NewCurveDialog::init()
{
    //Loads the data first
    QStringList dataPaths = Singleton<DataSingleton>::Instance().getExperimentalDataPaths();
    ui->dataLoaded->clear();
    if(!dataPaths.isEmpty()) {
        foreach(QString data, dataPaths) {
            ui->dataLoaded->addItem(QFileInfo(data).baseName(), data);
        }
        dataFileChanged(ui->dataLoaded->currentIndex());
    }

    ui->curvePenStyle->addItem(tr("Solid Line"), Qt::SolidLine);
    ui->curvePenStyle->addItem(tr("Dot Line"), Qt::DotLine);
    ui->curvePenStyle->addItem(tr("Dash Line"), Qt::DashLine);
    ui->curvePenStyle->addItem(tr("Dash and Dot Line"), Qt::DashDotLine);
    ui->curvePenStyle->addItem(tr("Dash Dot Dot Dash Line"), Qt::DashDotDotLine);


    ui->dataTable->setEditTriggers(QTableView::NoEditTriggers);

    mCurve = new Curve();
    mCurve->setType(Curve::Experimental);
}

void NewCurveDialog::loadFromCurve(Curve *curve)
{
    mEdit = true;
    mCurve = curve;
    if(mCurve != 0) {
        ui->curveName->setText(mCurve->title().text());
        ui->curveColor->setCurrentColor(mCurve->pen().color());
        ui->curveThickness->setValue(mCurve->pen().width());

        if(mCurve->getType() != Curve::Experimental) {
            ui->tabWidget->removeTab(1);
        } else {
            // XXX: load settings outside of curve, not very clean, but works
            QSettings *settings = Singleton<ProjectSingleton>::Instance().getSettings();
            settings->beginGroup("Curves/"+QString::number(curve->getId()));

            QString experimentPath = settings->value("data", "").toString();
            int index = ui->dataLoaded->findData(experimentPath);
            if(index != -1) {
                ui->dataLoaded->setCurrentIndex(index);
            }

            //QString abscissia = settings->value("abscissia", "").toString();
            //QString ordinate = settings->value("ordinate", "").toString();
            QString abscissia = mCurve->getExperimentalAbscissia();
            QString ordinate = mCurve->getExperimentalOrdinate();
            index = ui->dataAbscissia->findData(abscissia, Qt::UserRole);
            qDebug() << "abscissia: "<< abscissia << " at index " << index;
            if(index != -1) {
                ui->dataAbscissia->setCurrentIndex(index);
            }
            index = ui->dataOrdinate->findData(ordinate);
            qDebug() << "ordinate: "<< ordinate << " at index " << index;
            if(index != -1) {
                ui->dataOrdinate->setCurrentIndex(index);
            }
            settings->endGroup();
        }
    }
}

// ========================= SLOTS ============================
void NewCurveDialog::accept()
{
    qDebug() << "NewCurveDialog::accept()";
    QString title = ui->curveName->text();
    if(!mEdit && Singleton<CurveSingleton>::Instance().curveNameExists(title)) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Name Conflict"), tr("Another curve with the name ") + title + tr(" already exists. Do you want to modify the name?"), QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            mMayClose = false;
            return;
        } else {
            mMayClose = true;
        }
    } else {
        mMayClose = true;
    }

    mCurve->setTitle(ui->curveName->text());
    Qt::PenStyle penStyle = static_cast<Qt::PenStyle>(ui->curvePenStyle->itemData(ui->curvePenStyle->currentIndex()).toInt());
    QPen pen(ui->curveColor->currentColor());
    pen.setStyle(penStyle);
    pen.setWidth(ui->curveThickness->value());
    mCurve->setPen(pen);

    // Set curve data
    // If there is experimental data
    QString fileName = ui->dataLoaded->itemData(ui->dataLoaded->currentIndex()).toString();
    if(!fileName.isEmpty()) {
            QString abscissia = ui->dataAbscissia->itemData(ui->dataAbscissia->currentIndex()).toString();
            QString ordinate = ui->dataOrdinate->itemData(ui->dataOrdinate->currentIndex()).toString();
            if(!abscissia.isEmpty() && !ordinate.isEmpty()) {
                qDebug() << "NewCurveDialog::accept(): setting experimental curve data ("<<abscissia<<", "<<ordinate<<")" << endl;
                mCurve->setExperimentalData(fileName, abscissia, ordinate);
            }
    }


    if(mCurve->getType() == Curve::Experimental) {
        Singleton<CurveSingleton>::Instance().addCurve(mCurve);
    }

    if(mMayClose)
        QDialog::accept();
}

void NewCurveDialog::loadDataFile()
{
    // Load previously used directory when loading experimental data
    QSettings mSettings;
    QString startDir = mSettings.value("Save/experimentalDataDirectory", "").toString();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Experimental Data"), startDir, tr("Experimental Data Files(*.csv *.txt);;All Files (*.*)"));


    int index = ui->dataLoaded->findData(fileName);
    if(index != -1) {
        QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Data file already loaded"),
                                tr("This data file has already been loaded. If you have modified it in an external program, it might need to be loaded again.\n"
                                   "Do you want to load ")+fileName+tr(" again?"),
                                QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::Yes) {
            DataPtr data = Singleton<DataSingleton>::Instance().getData(fileName);
            QSharedPointer<ExperimentalData> d = qSharedPointerDynamicCast<ExperimentalData>(data);
            if(d != 0) {
                d->loadFromFile(fileName);
            }
        }
    }
    else {
        ExperimentalData *data = new ExperimentalData();
        data->loadFromFile(fileName);
        Singleton<DataSingleton>::Instance().addData(data);
        ui->dataLoaded->addItem(QFileInfo(fileName).baseName(), fileName);
    }

    // Save currently used directory for later use
    if(!fileName.isNull()) {
        mSettings.setValue("Save/experimentalDataDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
    }
}


void NewCurveDialog::dataFileChanged(int index)
{
    QString fileName = ui->dataLoaded->itemData(index).toString();
    qDebug() << "Data file changed: " << fileName;


    // Fill in the QComboBox for choosing abscissia and ordinate
    QSharedPointer<Data> data = Singleton<DataSingleton>::Instance().getData(fileName);
    if(data != 0) {
        QStringList columns = data->getAvailableColumns();
        foreach(QString column, columns) {
            ui->dataAbscissia->addItem(column, column);
            ui->dataOrdinate->addItem(column, column);
        }
        // Autoset abscissia to V column
        int ind = ui->dataAbscissia->findData("V");
        if(ind != -1) {
            ui->dataAbscissia->setCurrentIndex(ind);
        }

        // ========= Fill in the values in the tablewidget
        if(columns.length() > 0) {
            QStandardItemModel *model = new QStandardItemModel(this);
            model->setColumnCount(columns.length());
            for(int i=0; i<columns.length(); i++) {
                model->setHorizontalHeaderItem(i, new QStandardItem(columns[i]));
                DataColumn<double> col = data->getColumn(columns[i]);
                QVector<double> val = col.getData();
                for(int j=0; j<val.size(); j++) {
                    model->setItem(j, i, new QStandardItem(QString::number(val[j])));
                }
            }
            ui->dataTable->setModel(model);
        }
    }
}
