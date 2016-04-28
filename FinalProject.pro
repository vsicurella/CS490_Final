#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T12:09:28
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FinalProject
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    synth.cpp \
    wavetable.cpp

HEADERS  += mainwindow.h \
    synth.h \
    wavetable.h

FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs alsa --libs`
