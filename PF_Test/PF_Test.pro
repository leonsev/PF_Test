QT += widgets serialport

TARGET = PF_Test
TEMPLATE = app

HEADERS += \
    dialog.h \
    masterthread.h \
    pf_mcontroller.h \
    pf_retval.h \
    pf_rxprocessor.h \
    pf_crc.h \
    pf_rxbuffer.h

SOURCES += \
    main.cpp \
    dialog.cpp \
    masterthread.cpp \
    pf_mcontroller.cpp \
    pf_rxprocessor.cpp \
    pf_crc.cpp \
    pf_rxbuffer.cpp
