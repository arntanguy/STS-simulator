#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

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
    static QStandardItem *createFunctionItem(AbstractFunction *f);
    static void clearLayout(QLayout *layout);

    static mu::Parser* createParser();
};

#endif // HELPERFUNCTIONS_H
