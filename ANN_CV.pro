#-------------------------------------------------
#
# Project created by QtCreator 2015-01-13T18:51:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ANN_CV
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rna_ocv.cpp \
    XimeaCap.cpp

HEADERS  += mainwindow.h \
    rna_ocv.h \
    XimeaCap.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_calib3d

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_contrib

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_core

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_features2d

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_flann

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_highgui

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_legacy

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_ml

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_nonfree

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_objdetect

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_ocl

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_photo

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_stitching

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_superres

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_video

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lopencv_videostab

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include


unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lm3api

INCLUDEPATH += $$PWD/../../../../usr/include/m3api
DEPENDPATH += $$PWD/../../../../usr/include/m3api
