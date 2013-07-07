#include "helperfunctions.h"
#include <QStandardItem>
#include "abstractfunction.h"

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
