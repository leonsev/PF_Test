QT += widgets serialport

TARGET = PF_Test
TEMPLATE = app

#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += QT_NO_DEBUG
DEFINES += QTCSV_LIBRARY

CONFIG+= static

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/qtcsv/src/release/ -lqtcsv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../3rdparty/qtcsv/src/debug/ -lqtcsv
else:unix: LIBS += -L$$OUT_PWD/../3rdparty/qtcsv/src/ -lqtcsv

INCLUDEPATH += $$PWD/../3rdparty/qtcsv/src \
                $$PWD/../3rdparty/qtcsv/src/include
DEPENDPATH += $$PWD/../3rdparty/qtcsv/src


HEADERS += \
    dialog.h \
    #masterthread.h \
    #pf_mcontroller.h \
    pf_retval.h \
    #pf_rxprocessor.h \
    pf_crc.h \
    #pf_rxbuffer.h \
    pf_transmitter.h \
    pf_receiver.h \
    pf_adaptor.h \
    pf_error.h \
    pf_reply.h \
    pf_controller.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    #masterthread.cpp \
    #pf_mcontroller.cpp \
    #pf_rxprocessor.cpp \
    pf_crc.cpp \
    #pf_rxbuffer.cpp
    pf_transmitter.cpp \
    pf_receiver.cpp \
    pf_adaptor.cpp \
    pf_error.cpp \
    pf_reply.cpp \
    pf_controller.cpp



# Use .depends to specify that a project depends on another.
core.depends = qtcsv

