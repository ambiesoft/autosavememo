#-------------------------------------------------
#
# Project created by QtCreator 2019-02-10T19:50:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autosavememo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../../lsMisc/stdQt/settings.cpp \
    ../../lsMisc/stdQt/stdQt.cpp \
    ../../lsMisc/stdQt/stdQt_win32.cpp \
    mainwindow_serialize.cpp \
    mainwindow_my.cpp \
    mainwindow_cdtor.cpp

HEADERS += \
        mainwindow.h \
    consts.h \
    ../../lsMisc/stdQt/settings.h \
    ../../lsMisc/stdQt/stdQt.h \
    stable.h

FORMS += \
        mainwindow.ui

RC_FILE = app.rc

PRECOMPILED_HEADER = stable.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    app.rc \
    ../History.txt
