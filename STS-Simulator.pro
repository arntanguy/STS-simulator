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
win32 {
    include(C:/Qwt-6.1.0/qwt.prf)
    CONFIG += console
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
    functionvariableswidget.cpp \
    plotsingleton.cpp \
    integralfunction.cpp \
    integralcurve.cpp \
    integralfunctiondialog.cpp \
    globalconfigdialog.cpp \
    globalsettingssingleton.cpp \
    qthideandshowdialog.cpp \
    plotcontrolwindow.cpp \
    differentialcurve.cpp \
    differentialfunction.cpp \
    differentialfunctiondialog.cpp \
    functionhelpdialog.cpp \
    qsciencespinbox.cpp \
    csvfilewriter.cpp \
    plotexportdialog.cpp \
    sliderconfigdialog.cpp \
    slider.cpp \
    exportcurvesdialog.cpp \
    experimentalfunctiondialog.cpp \
    experimentalfunctionsingleton.cpp \
    experimentalfunction.cpp \
    transmissionfunction.cpp \
    transmissiondialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    plotwidget.h \
    legenditem.h \
    plotarea.h \
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
    functionvariableswidget.h \
    plotsingleton.h \
    integralfunction.h \
    integralcurve.h \
    integralfunctiondialog.h \
    globalconfigdialog.h \
    globalsettingssingleton.h \
    qthideandshowdialog.h \
    plotcontrolwindow.h \
    differentialcurve.h \
    PlotData.h \
    differentialfunction.h \
    differentialfunctiondialog.h \
    functionhelpdialog.h \
    qsciencespinbox.h \
    csvfilewriter.h \
    plotexportdialog.h \
    sliderconfigdialog.h \
    slider.h \
    exportcurvesdialog.h \
    experimentalfunctiondialog.h \
    experimentalfunctionsingleton.h \
    experimentalfunction.h \
    transmissionfunction.h \
    transmissiondialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    plotwidget.ui \
    plotarea.ui \
    newcurvedialog.ui \
    functionselectiondialog.ui \
    newfunctiondialog.ui \
        functionvariableswidget.ui \
    valueselector.ui \
    hierarchicalfunctiondialog.ui \
    integralfunctiondialog.ui \
    globalconfigdialog.ui \
    plotcontrolwindow.ui \
    differentialfunctiondialog.ui \
    functionhelpdialog.ui \
    plotexportdialog.ui \
    sliderconfigdialog.ui \
    exportcurvesdialog.ui \
    experimentalfunctiondialog.ui \
    transmissiondialog.ui

#unix {
#QMAKE_CXXFLAGS += -std=c++0x
#}


