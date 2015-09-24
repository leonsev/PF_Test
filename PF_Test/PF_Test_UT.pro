#-------------------------------------------------
#
# Project created by QtCreator 2015-08-29T13:21:38
#
#-------------------------------------------------

QT       += testlib serialport

QT       -= gui

TARGET = tst_pf_test_uttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += pf_rxprocessor.h \
            pf_retval.h \
            pf_mcontroller.h \
            pf_crc.h

SOURCES += tst_pf_test_uttest.cpp \
           pf_rxprocessor.cpp \
           pf_mcontroller.cpp \
            pf_crc.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"
