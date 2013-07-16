#include "helperfunctions.h"
#include "abstractfunction.h"

#include <QStandardItem>
#include <QLayout>
#include <QWidget>
#include <QDebug>

HelperFunctions::HelperFunctions()
{
}

QStandardItem* HelperFunctions::createFunctionItem(AbstractFunction *f)
{
    QStandardItem *item= new QStandardItem();
    item->setCheckable( true );
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
