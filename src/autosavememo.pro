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

INCLUDEPATH += ../compact_enc_det \

win32 {
    !exists( ../compact_enc_det/compact_enc_det/compact_enc_det.cc ) {
        error( "Submodule not found: run 'git submodule update -i'" )
    }
}

# See info/icu
win32-msvc* {
    MYICUDIR = icu.msvc
    message(../$$MYICUDIR/icu4c/bin/icuin.lib)
    exists( ../$$MYICUDIR/icu4c/bin/icuin.lib ) {
        MYICSVCLIB = -licutu -licuuc -licuio -licuin -licudt
    }
}
win32-g++ {
    MYICUDIR = icu.mingw810_32
    message(../$$MYICUDIR/icu4c/bin/libicuin.dll.a)
    exists(../$$MYICUDIR/icu4c/bin/libicuin.dll.a) {
        MYICSMINGWLIB = -llibicutu.dll -llibicuuc.dll -llibicudt.dll -llibicuin.dll -llibicuio.dll
    }
}


SOURCES += \
    ../../lsMisc/stdQt/waitingcursor.cpp \
        main.cpp \
        mainwindow.cpp \
    ../../lsMisc/stdQt/inisettings.cpp \
    ../../lsMisc/stdQt/stdQt.cpp \
    mainwindow_my.cpp \
    mainwindow_cdtor.cpp \
    ../compact_enc_det/util/languages/languages.cc \
    ../compact_enc_det/util/encodings/encodings.cc \
    ../compact_enc_det/compact_enc_det/compact_enc_det.cc \
    ../compact_enc_det/compact_enc_det/compact_enc_det_hint_code.cc \
    helper.cpp \
    autosavememoapp.cpp


HEADERS += \
    ../../lsMisc/stdQt/waitingcursor.h \
        mainwindow.h \
    consts.h \
    ../../lsMisc/stdQt/inisettings.h \
    ../../lsMisc/stdQt/stdQt.h \
    stable.h \
    ../compact_enc_det/compact_enc_det/compact_enc_det.h \
    helper.h \
    stdafx.h \
    autosavememoapp.h


FORMS += \
        mainwindow.ui

RC_FILE = app.rc

PRECOMPILED_HEADER = stable.h

# ..\compact_enc_det/compact_enc_det/compact_enc_det_generated_tables.h:1854:1: error: narrowing conversion of '139' from 'int' to 'char' inside { } [-Wnarrowing]
win32{
    HEADERS += ../../lsMisc/GetLastErrorString.h \

    SOURCES += ../../lsMisc/stdQt/stdQt_win32.cpp \
        ../../lsMisc/GetLastErrorString.cpp \
}
win32-g++ {
    QMAKE_CXXFLAGS += -Wno-narrowing
}
linux {
    QMAKE_CXXFLAGS += -Wno-narrowing
    SOURCES += ../../lsMisc/stdQt/stdQt_linux.cpp \
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    app.rc \
    ../History.txt


linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32:QMAKE_TARGET.arch = x86
linux-g++-64:QMAKE_TARGET.arch = x86_64

# ICU
win32-msvc* {
    message("MSVC")
    isEmpty(MYICSVCLIB) {
        # 1, git submodule update -i # this retrieve sources of the ICU.
        # 2, build
        # 3, copy lib/* to bin/
        # see info/ for more
        error( "ICU not ready." )
    }

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")
        LIBS += -L$$PWD/../$$MYICUDIR/icu4c/bin/ $$MYICSVCLIB

    } else {
        message("x86_64 build")
        LIBS += -L$$PWD/../$$MYICUDIR/icu4c/bin64/ $$MYICSVCLIB
    }
}
win32-g++ {
    message("win32-g++")
    isEmpty(MYICSMINGWLIB) {
        # 1, git submodule init -u # this retrieve sources of the ICU.
        # 2, build
        # 3, copy lib/* to bin/
        # see info/ for more
        error( "ICU not ready." )
    }

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")
        LIBS += -L$$PWD/../$$MYICUDIR/icu4c/bin/ $$MYICSMINGWLIB

    } else {
        message("x86_64 build")
        LIBS += -L$$PWD/../$$MYICUDIR/icu4c/bin64/ $$MYICSMINGWLIB
    }
}
linux {
    LIBS += -L/usr/lib/x86_64-linux-gnu -licui18n -licuuc -licudata
}

# System libs
win32-msvc* {
    LIBS += Shlwapi.lib
}
win32-g++ {
    LIBS += -lshlwapi -lpsapi
}
message($$LIBS)

INCLUDEPATH += $$PWD/../$$MYICUDIR/icu4c/include
DEPENDPATH += $$PWD/../$$MYICUDIR/icu4c/include

message($$INCLUDEPATH)

RESOURCES += \
    resource.qrc
