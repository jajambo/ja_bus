QT += core
QT -= gui
QT += serialport
INCLUDEPATH = C:\Users\jajambol\Documents\Arduino\libraries\ja_bus
TARGET = rs232_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    qt_ja_commander_thread.cpp \
    qt_ja_led_control.cpp \
    qt_ja_device.cpp

HEADERS += \
    serialread.h \
    qt_ja_led_control.h \
    qt_ja_device.h \
    qt_ja_request.h \
    qt_ja_commander_thread.h \
    ja_host.h

