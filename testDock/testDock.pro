#-------------------------------------------------
#
# Project created by QtCreator 2013-04-08T12:59:04
#
#-------------------------------------------------
 
QT       += core gui
 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
 
TARGET = testDock
TEMPLATE = app
 
 
SOURCES += main.cpp\
        mainwindow.cpp \
    myDockTitleBar.cpp
 
HEADERS  += mainwindow.h \
    myDockTitleBar.h
 
FORMS    += \
    mainwindow.ui