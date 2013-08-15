#ifndef EXPERIMENTALFUNCTIONSINGLETON_H
#define EXPERIMENTALFUNCTIONSINGLETON_H

// WARNING, if this number is changed, please take care of defining all "trampoline functions" correctly.
// It won't work otherwise!!
#define NUMBER_OF_DZ 10

#include <QMap>
#include "plotdata.h"
#include "experimentalfunction.h"

namespace mu
{
    class Parser;
}

template<int I> double dz(double x);

/***
 * WARNING: read this carefully
 * This singleton uses a C++ concept called trampoline function to generate at compile time several different
 * c++ functions, to be used as callbacks.
 * The template argument in the function (<int I>), is used as an identifier to know which data file to use.
 * While not optimal, it's the best way I've found to solve the problem of linking several differnt mu::Parser
 * funcitons to the "same" one (can't instanciate functions at runtime, so it had to somehow be done at compile time
 * but without modifying the function signature.
 * I needed to create functions of type double DZ1(double), so that I could link it to mu::Parser with
 * Parser.DefineFun("DZ1", DZ1);
 * Adding an id to the function would be unacceptable...
 *
 * See the topic http://stackoverflow.com/questions/12404307/c-dynamically-define-function, where I got the idea from
 **/
class ExperimentalFunctionSingleton
{
public:
    ExperimentalFunctionSingleton();

    bool linkData(int dzIndex, const QString &experimentId, const QString &abscissiaColumnName, const QString &ordinateColumnName);
    bool hasLinkedData(int dzIndex);

    double interpolate(int dzIndex, double x);

    void defineParserFunctions(mu::Parser *parser);

    QMap<int, ExperimentalFunction *> getAllFunctions() {
        return mFunctions;
    }

private:
    QMap<int, ExperimentalFunction *> mFunctions;
};

#endif // EXPERIMENTALFUNCTIONSINGLETON_H
