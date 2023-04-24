#-------------------------------------------------
#
# Project created by QtCreator 2021-04-09T09:10:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpindleMonitor
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

!contains(QMAKE_TARGET.arch, x86_64) {
    XW_32_64 = Win32
} else {
    XW_32_64 = x64
}

GUI_PATH = ../../esviGUI
GUI_3RD = $${GUI_PATH}/3rd

#相机头文件
INCLUDEPATH += $${GUI_3RD}/SaperaSDK/Include
INCLUDEPATH += $${GUI_3RD}/SaperaSDK/Include/Classes/Basic
#运动控制卡头文件
INCLUDEPATH += $${GUI_3RD}/zmotion/$${XW_32_64}
#相机库
LIBS += -L$${GUI_3RD}/SaperaSDK/Lib -lSapClassBasic
#运动控制卡
LIBS += -L$${GUI_3RD}/zmotion/$${XW_32_64} -lzauxdll

#图像处理
ALG_PATH = ../../esviALG
ALG_3RD = $${ALG_PATH}/3rd

INCLUDEPATH += $${ALG_3RD}/ZcLib/include
INCLUDEPATH += $${ALG_3RD}/ZcLib/include/imageprocess
INCLUDEPATH += $${ALG_3RD}/ZcLib/include/measure
INCLUDEPATH += $${ALG_3RD}/ZcLib/include/Samples

CONFIG(debug, debug|release){
    ALG_LIB_PATH = $${ALG_3RD}/ZcLib/lib/x64/Debug

    LIBS += -L$${ALG_LIB_PATH} -lZcAlg_Qtd
    LIBS += -L$${ALG_LIB_PATH} -lZcAlg_OpenCvd
    LIBS += -L$${ALG_LIB_PATH} -lZcAlgd
} else {
    ALG_LIB_PATH = $${ALG_3RD}/ZcLib/lib/x64/Release

    LIBS += -L$${ALG_LIB_PATH} -lZcAlg_Qt
    LIBS += -L$${ALG_LIB_PATH} -lZcAlg_OpenCv
    LIBS += -L$${ALG_LIB_PATH} -lZcAlg
}

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    acqconfig.cpp \
    imagedeal.cpp \
    sportcontrol.cpp

HEADERS += \
        mainwindow.h \
    acqconfig.h \
    imagedeal.h \
    sportcontrol.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
