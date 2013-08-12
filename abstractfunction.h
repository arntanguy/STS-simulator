#ifndef ABSTRACTFUNCTION_H
#define ABSTRACTFUNCTION_H

#include <QString>
#include <QMetaType>
#include <QObject>
#include <QMap>

#include "typedefs.h"

class PlotWidget;
class FunctionCurve;

class AbstractFunction : public QObject
{
    Q_OBJECT

    public:
        /**
         * Bitfield used to filter functions by type.
         * Set flags like flag = Function | HierachicalFunction;
         * Check them with if(flag & HierarchicalFunction)
         **/
        enum FunctionType {
            Function = 1 << 0,   // binary 0001
            HierarchicalFunction = 1 << 1, // binary 0010
            Integral = 1 << 2, // binary 0100
            Differential = 1 << 3,
            Unknown = 1 << 4,
            All = ~((unsigned int)0)
        };
        friend FunctionType operator|(FunctionType a, FunctionType b);

    public:
        AbstractFunction(QObject *parent=0);
        AbstractFunction(int id, QObject *parent=0);
        AbstractFunction(const AbstractFunction&);

        void setName(const QString &name);
        QString getName() const;
        QString getVariable() const;
        void setType(AbstractFunction::FunctionType type);
        AbstractFunction::FunctionType getType() const;
        QString getTypeStr() const;

        /**
         * Virtual functions. Every function must have these to be a valid function
         **/
        virtual double compute(const QString&, double) = 0;
        virtual double compute(double) = 0;
        virtual double operator() (double x);
        virtual QString getExpression() const = 0;
        virtual void save(const QString &group);
        virtual void loadFromConfig(const QString &group);
        virtual FunctionCurve* createCurve();
        void setCurve(FunctionCurve *curve);
        FunctionCurve* getCurve();

        unsigned int getCurveId() const;
        void updateLinkedCurve(bool forceUpdate = false);

        bool isDisplayed(unsigned int plotId) const;

        void setId(int id);
        int getId() const {
            return mFunctionId;
        }


        static void resetStaticId();
Q_SIGNALS:
        void nameUpdated(const QString &);
        virtual void curveUpdated(AbstractFunction *);
        virtual void functionUpdated();
        void needsRecompute();

        public Q_SLOTS:
            void updateLinkedCurve(QString var, double val, bool forceUpdate = false);

    protected:
        void abstractsave(const QString &);

    private:
        static int mFunctionStaticId;
        int mFunctionId;

    protected:
        QString mName;
        QString mVariable;
        FunctionType mType;

        bool mNeedsUpdate;

        FunctionCurve *mLinkedCurve;
};

//Q_DECLARE_METATYPE(AbstractFunction*);
Q_DECLARE_METATYPE(AbstractFunctionPtr);

#endif // ABSTRACTFUNCTION_H
