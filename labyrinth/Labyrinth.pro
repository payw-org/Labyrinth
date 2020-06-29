#-------------------------------------------------
#
# Project created by QtCreator 2016-12-09T01:57:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Labyrinth
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MasterID.cpp \
    NormalID.cpp \
    Manager.cpp \
    LQListwidgetItem.cpp

HEADERS  += mainwindow.h \
    NormalID.h \
    MasterID.h \
    Manager.h \
    LQListwidgetItem.h \
    Encryption.h \
    File.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

ICON = labyrinthicon.icns
