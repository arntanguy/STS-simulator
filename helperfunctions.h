#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "typedefs.h"

class QStandardItem;
class AbstractFunction;
class QLayout;
namespace mu {
    class Parser;
}

class HelperFunctions
{
public:
    HelperFunctions();
    static QStandardItem *createFunctionItem(AbstractFunctionPtr f, bool checkable = true);
    static void clearLayout(QLayout *layout);

    static mu::Parser* createParser();
};

#endif // HELPERFUNCTIONS_H
