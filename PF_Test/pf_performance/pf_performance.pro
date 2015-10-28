QT += widgets serialport

TARGET = PF_Test
TEMPLATE = app

#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += QT_NO_DEBUG
DEFINES += QTCSV_LIBRARY

CONFIG+= static

INCLUDEPATH += $$PWD/../3rdparty/qtcsv/src \
                $$PWD/../3rdparty/qtcsv/src/include \
                $$PWD/../pf_core
DEPENDPATH += $$PWD/../3rdparty/qtcsv/src \
                $$PWD/../pf_core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../pf_core/release/ -lpf_core -L$$OUT_PWD/../3rdparty/qtcsv/src/release/ -lqtcsv
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../pf_core/debug/ -lpf_core -L$$OUT_PWD/../3rdparty/qtcsv/src/debug/ -lqtcsv
else:unix: LIBS += -L$$OUT_PWD/../pf_core/ -lpf_core -L$$OUT_PWD/../3rdparty/qtcsv/src/ -lqtcsv
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../pf_core/release/libpf_core.a $$OUT_PWD/../3rdparty/qtcsv/src/release/libqtcsv.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../pf_core/debug/libpf_core.a $$OUT_PWD/../3rdparty/qtcsv/src/debug/libqtcsv.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../pf_core/release/pf_core.lib $$OUT_PWD/../3rdparty/qtcsv/src/release/libqtcsv.a
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../pf_core/debug/pf_core.lib $$OUT_PWD/../3rdparty/qtcsv/src/debug/libqtcsv.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../pf_core/libpf_core.a $$OUT_PWD/../3rdparty/qtcsv/src/libqtcsv.a

HEADERS += \
    dialog.h

SOURCES += \
    main.cpp \
    dialog.cpp



