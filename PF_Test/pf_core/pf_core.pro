#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T19:33:00
#
#-------------------------------------------------

QT       -= gui
QT       += serialport

TARGET = pf_core
TEMPLATE = lib
CONFIG += staticlib

SOURCES += pf_crc.cpp \
    pf_transmitter.cpp \
    pf_receiver.cpp \
    pf_adaptor.cpp \
    pf_error.cpp \
    pf_reply.cpp \
    pf_controller.cpp

HEADERS += pf_retval.h \
    pf_crc.h \
    pf_transmitter.h \
    pf_receiver.h \
    pf_adaptor.h \
    pf_error.h \
    pf_reply.h \
    pf_controller.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
