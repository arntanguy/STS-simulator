#include "functionhelpdialog.h"
#include "ui_functionhelpdialog.h"
#include "muParser.h"
#include "helperfunctions.h"

FunctionHelpDialog::FunctionHelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FunctionHelpDialog)
{
    ui->setupUi(this);
    init();
}

FunctionHelpDialog::~FunctionHelpDialog()
{
    delete ui;
}


/**
 * Get the list of all available functions and constants from the parser,
 * and display it on the interface
 * **/
void FunctionHelpDialog::init()
{
    mParser = HelperFunctions::createParser();
    mu::valmap_type cmap = mParser->GetConst();
    QString constStr;
    if (cmap.size())
    {
        mu::valmap_type::const_iterator item = cmap.begin();
        for (; item!=cmap.end(); ++item) {
            constStr += QString(item->first.c_str()) + " = " + QString::number(item->second) + "\n";
        }
    }
    ui->constants->setText(constStr);

    mu::funmap_type fmap = mParser->GetFunDef();
    QString funStr;
    if (fmap.size())
    {
        mu::funmap_type::const_iterator item = fmap.begin();
        for (; item!=fmap.end(); ++item) {
            funStr += QString(item->first.c_str()) + "\n";
        }
    }
    ui->functions->setText(funStr);
}
