#include "helperfunctions.h"
#include "abstractfunction.h"
#include "muParser.h"
#include "experimentalfunctionsingleton.h"
#include "singleton.h"

#include <QStandardItem>
#include <QLayout>
#include <QWidget>
#include <QDebug>

HelperFunctions::HelperFunctions()
{
}

QStandardItem* HelperFunctions::createFunctionItem(AbstractFunctionPtr f, bool checkable)
{
    QStandardItem *item= new QStandardItem();
    item->setCheckable( checkable );
    //item->setCheckState(Qt::Unchecked);
    item->setEditable(false);
    item->setText(f->getName());
    item->setData(QVariant::fromValue(f), Qt::UserRole);
    return item;
}

void HelperFunctions::clearLayout(QLayout *layout)
{
    qDebug() << "HelperFunctions::clearLayout()";
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            item->widget()->hide();
            delete item->widget();
        }
        layout->removeItem(item);
        delete item;
    }
}

mu::Parser* HelperFunctions::createParser()
{
    mu::Parser* parser = new mu::Parser();
    parser->DefineConst("_pi", (double)3.14159);
    Singleton<ExperimentalFunctionSingleton>::Instance().defineParserFunctions(parser);
    return parser;
}
