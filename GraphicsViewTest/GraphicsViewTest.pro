#-------------------------------------------------
#
# Project created by QtCreator 2018-10-13T22:47:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicsViewTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lineitem.cpp \
    rectitem.cpp \
    widgetitem.cpp

HEADERS  += mainwindow.h \
    lineitem.h \
    rectitem.h \
    widgetitem.h

FORMS    += mainwindow.ui
