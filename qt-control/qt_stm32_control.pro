QT += core gui widgets serialport

CONFIG += c++17 warn_on
TEMPLATE = app
TARGET = qt_stm32_control

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    serialcontroller.cpp

HEADERS += \
    mainwindow.h \
    serialcontroller.h

