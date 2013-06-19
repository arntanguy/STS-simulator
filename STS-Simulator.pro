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
    plotwidget.cpp \
    legenditem.cpp \
    plotarea.cpp \
    plotcontroldialog.cpp \
    curve.cpp \
    csvfilereader.cpp \
    csvexperimentaldatareader.cpp \
    datacolumn.cpp \
    projectsingleton.cpp \
    newcurvedialog.cpp \
    qtcolorpicker.cpp \
    curvesingleton.cpp \
    datasingleton.cpp \
    data.cpp \
    experimentaldata.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    plotwidget.h \
    legenditem.h \
    plotarea.h \
    plotcontroldialog.h \
    randomcurve.h \
    curve.h \
    csvfilereader.h \
    csvexperimentaldatareader.h \
    datacolumn.h \
    singleton.h \
    projectsingleton.h \
    newcurvedialog.h \
    qtcolorpicker.h \
    curvesingleton.h \
    datasingleton.h \
    data.h \
    experimentaldata.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    plotwidget.ui \
    plotarea.ui \
    plotcontroldialog.ui \
    newcurvedialog.ui

INCLUDEPATH += /usr/local/qwt-6.1.0-rc3/include
LIBS +=  -L/usr/local/qwt-6.1.0-rc3/lib -lqwt

QMAKE_CXXFLAGS += -std=c++0x
