#ifndef __typedefs__
#define __typedefs__

#include <QSharedPointer>

class AbstractFunction;
class Function;
class HierarchicalFunction;
class IntegralFunction;
class DifferentialFunction;
class Data;

typedef QSharedPointer<Function> FunctionPtr;
typedef QSharedPointer<AbstractFunction> AbstractFunctionPtr;
typedef QSharedPointer<HierarchicalFunction> HierarchicalFunctionPtr;
typedef QSharedPointer<IntegralFunction> IntegralFunctionPtr;
typedef QSharedPointer<DifferentialFunction> DifferentialFunctionPtr;
typedef QSharedPointer<Data> DataPtr;

#endif
