#-------------------------------------------------
#
# Project created by QtCreator 2017-01-11T14:31:43
#
#-------------------------------------------------
QT += core gui widgets xml


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestMode
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    domparse.cpp

HEADERS  += mainwindow.h \
    domparse.h

FORMS    += mainwindow.ui

RESOURCES += \
    src.qrc
