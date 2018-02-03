#-------------------------------------------------
#
# Project created by QtCreator 2018-01-27T20:13:17
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = SWJC_SERVER
TEMPLATE    = app
#MOC_DIR     = temp/moc
#RCC_DIR     = temp/rcc
#UI_DIR      = temp/ui
#OBJECTS_DIR = temp/obj
DESTDIR     = $$PWD/../../bin/swjc_server

DEFINES += QT_DEPRECATED_WARNINGS



SOURCES += main.cpp \
    mainWidget.cpp \
    common/st_logger.cpp

HEADERS  += \
    mainWidget.h \
    common/st_logger.h \
    common/comm.h

FORMS    += \
    mainWidget.ui

RESOURCES += \
    image.qrc \
    qss.qrc
