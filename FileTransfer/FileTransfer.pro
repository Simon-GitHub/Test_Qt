QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += \
    main.cpp \
    Server.cpp \
    Client.cpp \
    MainWidget.cpp \
    ProgressBar.cpp

HEADERS += \
    Server.h \
    Client.h \
    MainWidget.h \
    ProgressBar.h

OTHER_FILES += \
    FileTransfer
