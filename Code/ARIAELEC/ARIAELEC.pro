#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T01:07:12
#
#-------------------------------------------------

QT       += core gui
QT += serialport
CONFIG += qwt
QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ARIAELEC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    curvedata.cpp \
    curvedata2.cpp \
    knob.cpp \
    knob2.cpp \
    plot.cpp \
    plot2.cpp \
    samplingthread.cpp \
    samplingthread2.cpp \
    signaldata.cpp \
    signaldata2.cpp \
    wheelbox.cpp \
    wheelbox2.cpp

HEADERS  += mainwindow.h \
    curvedata.h \
    curvedata2.h \
    knob.h \
    knob2.h \
    plot.h \
    samplingthread.h \
    samplingthread2.h \
    signaldata.h \
    signaldata2.h \
    wheelbox.h \
    wheelbox2.h \
    plot2.h

FORMS    += mainwindow.ui

#win32: RC_ICONS = AriaElec.ico

RESOURCES +=
