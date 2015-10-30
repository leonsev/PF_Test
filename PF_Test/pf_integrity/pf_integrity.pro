#-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T21:18:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pf_integrity
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    pf_noize.cpp \
    pf_testsuit.cpp \
    pf_errresult.cpp \
    pf_testcontroller.cpp


HEADERS  += dialog.h \
    pf_noize.h \
    pf_testsuit.h \
    pf_errresult.h \
    pf_testcontroller.h

FORMS    += dialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../pf_core/release/ -lpf_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../pf_core/debug/ -lpf_core
else:unix: LIBS += -L$$OUT_PWD/../pf_core/ -lpf_core

INCLUDEPATH += $$PWD/../pf_core
DEPENDPATH += $$PWD/../pf_core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../pf_core/release/libpf_core.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../pf_core/debug/libpf_core.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../pf_core/release/pf_core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../pf_core/debug/pf_core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../pf_core/libpf_core.a
