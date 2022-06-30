#-------------------------------------------------
#
# Project created by QtCreator 2021-05-20T16:23:33
#
#-------------------------------------------------

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoLightCamera
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

#release新添加生成日志
DEFINES += QT_MESSAGELOGCONTEXT

CONFIG += c++11

#INCLUDEPATH +=  ../../esviALG_local/include \
#                ../../esviGUI_local/include
#INCLUDEPATH += ../../esviALG_light/3rd/eslib64/include
#INCLUDEPATH += ../../esviALG_light/3rd/ZcLib_jiu/include
#win32:{
#contains(QMAKE_TARGET.arch, x86_64) {
#    CONFIG(release, debug|release):{
#        LIBS += -L$$PWD/../../esviGUI_local/lib/MSVC/x64/release/ -lesviRConn
#        LIBS += -L$$PWD/../../esviALG_light/3rd/eslib64/lib/ -lvxocl64 -lParaObj64
#        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib_jiu/lib/x64/Release/ -lZcAlg_OpenCv
#        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib_jiu/lib/x64/Release/ -lZcAlg
##        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib_jiu/lib/x64/Release/ -lZcAlg_Qt
##        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib_jiu/lib/x64/Release/ -lZcCamera
#        LIBS += -L$$PWD/../../esviALG/3rd/opencv_3_4_3/x64/vc15/lib/Release/ -lopencv_core343 -lopencv_imgproc343
#    }else:CONFIG(debug, debug|release):{
#        LIBS += -L$$PWD/../../esviGUI_local/lib/MSVC/x64/debug/ -lesviRConnd
#        LIBS += -L$$PWD/../../esviALG_light/3rd/eslib64/lib/ -lvxocl64 -lParaObj64
#        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib_jiu/lib/x64/Debug/ -lZcAlg_OpenCvd
#        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib_jiu/lib/x64/Debug/ -lZcAlgd
##        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib_jiu/lib/x64/Debug/ -lZcAlg_Qtd
##        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib_jiu/lib/x64/Debug/ -lZcCamerad
#        LIBS += -L$$PWD/../../esviALG/3rd/opencv_3_4_3/x64/vc15/lib/Debug/ -lopencv_core343d  -lopencv_imgproc343d
#    }
#}}


INCLUDEPATH +=  ../../esviALG_local/include \
                ../../esviGUI_local/include
INCLUDEPATH += ../../esviALG_light/3rd/eslib64/include
INCLUDEPATH += ../../esviALG_light/3rd/ZcLib/include
win32:{
contains(QMAKE_TARGET.arch, x86_64) {
    CONFIG(release, debug|release):{
        LIBS += -L$$PWD/../../esviGUI_local/lib/MSVC/x64/release/ -lesviRConn
        LIBS += -L$$PWD/../../esviALG_light/3rd/eslib64/lib/ -lvxocl64 -lParaObj64
        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib/lib/x64/Release/ -lZcAlg_OpenCv
        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib/lib/x64/Release/ -lZcAlg
#        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib/lib/x64/Release/ -lZcAlg_Qt
#        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib/lib/x64/Release/ -lZcCamera
        LIBS += -L$$PWD/../../esviALG_local/3rd/opencv_3_4_3/x64/vc15/lib/Release/ -lopencv_core343 -lopencv_imgproc343
    }else:CONFIG(debug, debug|release):{
        LIBS += -L$$PWD/../../esviGUI_local/lib/MSVC/x64/debug/ -lesviRConnd
        LIBS += -L$$PWD/../../esviALG_light/3rd/eslib64/lib/ -lvxocl64 -lParaObj64
        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib/lib/x64/Debug/ -lZcAlg_OpenCvd
        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib/lib/x64/Debug/ -lZcAlgd
#        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib/lib/x64/Debug/ -lZcAlg_Qtd
#        LIBS += -L$$PWD/../../esviALG_light/3rd/ZcLib/lib/x64/Debug/ -lZcCamerad
        LIBS += -L$$PWD/../../esviALG_local/3rd/opencv_3_4_3/x64/vc15/lib/Debug/ -lopencv_core343d  -lopencv_imgproc343d
    }
}}


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    camlightdlg.cpp \
    CStorageInterface.cpp \
    imageform.cpp \
    graphicsview.cpp \
    toolrect.cpp \
    graphicspixmapitem.cpp \
    waitdialog.cpp

HEADERS += \
        mainwindow.h \
    camlightdlg.h \
    CStorageInterface.h \
    imageform.h \
    graphicsview.h \
    toolrect.h \
    graphicspixmapitem.h \
    waitdialog.h

FORMS += \
        mainwindow.ui \
    camlightdlg.ui \
    imageform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
