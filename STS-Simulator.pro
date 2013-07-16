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
#SUBDIRS = libs/muparser

# Set path to qwt on linux
unix {
    INCLUDEPATH += /usr/local/qwt-6.1.0-rc3/include
    LIBS +=  -L/usr/local/qwt-6.1.0-rc3/lib -lqwt
}


## To build muparser from included sources
include(libs/muparser/muparser.pri)
## To build muparser with pkgconfig
#unix {
#    CONFIG += link_pkgconfig
#    #PKGCONFIG file without extension
#    PKGCONFIG += /usr/local/lib/pkgconfig/muparser.pc
#}



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
    experimentaldata.cpp \
    functionselectiondialog.cpp \
    newfunctiondialog.cpp \
    functionssingleton.cpp \
    function.cpp \
    variablefactory.cpp \
    valueselector.cpp \
    hierarchicalfunction.cpp \
    abstractfunction.cpp \
    helperfunctions.cpp \
    hierarchicalfunctiondialog.cpp \
    functioncurve.cpp \
    functionfactory.cpp \
    unittest.cpp \
    functionvariableswidget.cpp

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
    experimentaldata.h \
    functionselectiondialog.h \
    newfunctiondialog.h \
    functionssingleton.h \
    function.h \
    variablefactory.h \
    valueselector.h \
    hierarchicalfunction.h \
    abstractfunction.h \
    helperfunctions.h \
    hierarchicalfunctiondialog.h \
    functioncurve.h \
    functionfactory.h \
    unittest.h \
    functionvariableswidget.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    plotwidget.ui \
    plotarea.ui \
    plotcontroldialog.ui \
    newcurvedialog.ui \
    functionselectiondialog.ui \
    newfunctiondialog.ui \
        functionvariableswidget.ui \
    valueselector.ui \
    hierarchicalfunctiondialog.ui

QMAKE_CXXFLAGS += -std=c++0x

