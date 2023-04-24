#-------------------------------------------------
#
# Project created by QtCreator 2021-12-21T21:41:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestSegmentImage
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

ESLIB_PATH = $$PWD/../../eslib64
ALG_PATH = $$PWD/../../esviALG_local

INCLUDEPATH += $${ALG_PATH}/3rd/opencv_3_4_3/include\
               $${ESLIB_PATH}/include

LIBS += -L$${ESLIB_PATH}/lib -lvxocl64
LIBS += -L$${ALG_PATH}/3rd/opencv_3_4_3/x64/vc15/lib/Release/ -lopencv_core343 -lopencv_imgproc343

SOURCES += \
        main.cpp \
        widget.cpp \
    graphicsview.cpp \
    graphicsscene.cpp \
    extvariation3chitem.cpp \
    extvariation3chtask.cpp \
    dugroupdefect.cpp \
    extpolygonitem.cpp \
    ducontour.cpp \
    esvidataitem.cpp

HEADERS += \
        widget.h \
    graphicsview.h \
    graphicsscene.h \
    extvariation3chitem.h \
    extvariation3chtask.h \
    dutypes.h \
    dugroupdefect.h \
    extpolygonitem.h \
    ducontour.h \
    esvidataitem.h \
    abstractdisplay.h

FORMS += \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
