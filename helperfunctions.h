#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

class QStandardItem;
class AbstractFunction;
class QLayout;

class HelperFunctions
{
public:
    HelperFunctions();
    static QStandardItem *createFunctionItem(AbstractFunction *f);
    static void clearLayout(QLayout *layout);
};

#endif // HELPERFUNCTIONS_H
