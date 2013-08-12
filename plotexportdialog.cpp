#include "plotexportdialog.h"
#include "ui_plotexportdialog.h"

#include <QSettings>
#include <QFileInfo>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <qwt_plot_renderer.h>

PlotExportDialog::PlotExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotExportDialog)
{
    ui->setupUi(this);

    ui->pageSize->addItem("A4", QPrinter::A4);
    ui->pageSize->addItem("A3", QPrinter::A3);

    ui->layout->addItem("Combined on one page", 0);
    ui->layout->addItem("One plot per page", 1);

    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

void PlotExportDialog::addPlot(QwtPlot *p)
{
    if(p != 0)
        mPlots.append(p);
}

PlotExportDialog::~PlotExportDialog()
{
    delete ui;
}

void PlotExportDialog::chooseFile()
{
    // Load previously used directory when loading experimental data
    QSettings settings;
    QString startDir = settings.value("Save/exportPlotDirectory", "").toString();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Project location"),
                                                    startDir,
                                                    tr("PDF (*.pdf);;All Files (*.*);;All (*)"));
    if(!fileName.isEmpty()) {
        settings.setValue("Save/exportPlotDirectory", QFileInfo(fileName).absoluteDir().absolutePath());
        ui->filePath->setText(fileName);
    }
}

void PlotExportDialog::accept()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(ui->filePath->text());

    if(ui->landscape->checkState() == Qt::Checked) {
        printer.setOrientation(QPrinter::Landscape);
    } else {
        printer.setOrientation(QPrinter::Portrait);
    }
    printer.setPaperSize(static_cast<QPrinter::PaperSize>(ui->pageSize->itemData(ui->pageSize->currentIndex()).toUInt()));
    int type = ui->layout->itemData(ui->layout->currentIndex()).toUInt();
    QRectF pageRect = printer.pageRect();
    QPainter painter(&printer);
    QwtPlotRenderer renderer;
    renderer.setDiscardFlags(QwtPlotRenderer::DiscardBackground | QwtPlotRenderer::DiscardCanvasFrame );

    if(type == 0) {
        QRectF rect[4];
        float margin = 100;
        float halfMargin = margin/2;
        rect[0] = QRectF(margin, margin, pageRect.width()/2-halfMargin, pageRect.height()/2-halfMargin);
        rect[1] = QRectF(margin, margin+pageRect.height()/2+halfMargin, pageRect.width()/2-halfMargin, pageRect.height()/2-margin);
        rect[2] = QRectF(margin+pageRect.width()/2+halfMargin, margin, pageRect.width()/2-halfMargin, pageRect.height()/2-halfMargin);
        rect[3] = QRectF(margin+pageRect.width()/2+halfMargin, margin+pageRect.height()/2+halfMargin, pageRect.width()/2-halfMargin, pageRect.height()/2-margin);

        int i = 0;
        foreach(QwtPlot *p, mPlots) {
            if(i<4) {
                renderer.render(p, &painter, rect[i++]);
            }
        }
    } else {
        int i = 0;
        foreach(QwtPlot *p, mPlots) {
            renderer.render(p, &painter, QRectF(0,0, pageRect.width(), pageRect.height()));
            if(i++ < 3)
                printer.newPage();
        }
    }

    QDialog::accept();
}
