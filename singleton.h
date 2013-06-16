#ifndef SINGLETON_H
#define SINGLETON_H

#include <QObject>

/*!
 * Singleton class template. You can make any class a singleton using this class template:
 *
 *  // mycode.cpp
 *
 *  #include "singleton.h"
 *  #include "MySingletonClass.h"
 *
 *  MySingletonClass *singleton = &Singleton<MySingletonClass>::Instance();
 *
 *  singleton->storeInfo();
 */
template <class T>
class Singleton
{
public:
    static T& Instance()
    {
        static T _instance; // create static instance of our class
        return _instance;   // return it
    }

private:
    Singleton();	// hide constructor
    ~Singleton();	// hide destructor
    Singleton(const Singleton &); // hide copy constructor
    Singleton& operator=(const Singleton &); // hide assign op
};

#endif // SINGLETON_H
