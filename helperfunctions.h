#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

class QStandardItem;
class AbstractFunction;

class HelperFunctions
{
public:
    HelperFunctions();
    static QStandardItem *createFunctionItem(AbstractFunction *f);
};

#endif // HELPERFUNCTIONS_H
