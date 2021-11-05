#-------------------------------------------------
#
# Project created by QtCreator 2021-03-30T09:04:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XEst
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../other_lib/opencv/build/x64/vc15/lib/ -lopencv_world343
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../other_lib/opencv/build/x64/vc15/lib/ -lopencv_world343d

INCLUDEPATH += $$PWD/../../other_lib/opencv/build/include

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    WidgetTDisplay.cpp \
    AppConLaser.cpp \
    WidgetTFunBtns.cpp \
    PageProject.cpp \
    PageRun.cpp \
    WidgetTabInfo.cpp \
    PageImage.cpp

HEADERS += \
        mainwindow.h \
    WidgetTDisplay.h \
    AppConLaser.h \
    WidgetTFunBtns.h \
    PageProject.h \
    PageRun.h \
    XEstUiDef.h \
    WidgetTabInfo.h \
    PageImage.h

FORMS += \
        mainwindow.ui \
    WidgetTDisplay.ui \
    WidgetTFunBtns.ui \
    PageProject.ui \
    PageRun.ui \
    WidgetTabInfo.ui \
    PageImage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

