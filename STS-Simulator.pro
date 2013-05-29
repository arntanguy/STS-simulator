#-------------------------------------------------
#
# Project created by QtCreator 2013-05-25T10:18:01
#
#-------------------------------------------------

QT       += core gui
CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = STS-Simulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    plotwidget.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    plotwidget.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    plotwidget.ui

INCLUDEPATH += /usr/local/qwt-6.1.0-rc3/include
LIBS +=  -L/usr/local/qwt-6.1.0-rc3/lib -lqwt
