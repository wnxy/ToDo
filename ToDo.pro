QT       += core gui
QT       += sql
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# 引入Log4QT开源日志库，定义所需的宏
DEFINES += LOG4QT_LIBRARY

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CommonWidget/Sources/colorplate.cpp \
    common.cpp \
    customitem.cpp \
    datamanager.cpp \
    inforstate.cpp \
    log.cpp \
    main.cpp \
    mainwindow.cpp \
    syssetwindow.cpp \
    systemtray.cpp

HEADERS += \
    CommonWidget/Headers/colorplate.h \
    common.h \
    customitem.h \
    datamanager.h \
    inforstate.h \
    log.h \
    mainwindow.h \
    syssetwindow.h \
    systemtray.h

FORMS += \
    inforstate.ui \
    mainwindow.ui \
    syssetwindow.ui

# 定义 Log4Qt 源码根目录
LOG4QT_ROOT_PATH = $$PWD/../Log4Qt-master

# 指定编译项目时应该被搜索的 #include 目录
INCLUDEPATH += $$LOG4QT_ROOT_PATH/src \
               $$LOG4QT_ROOT_PATH/src/log4qt \
               $$LOG4QT_ROOT_PATH/include \
               $$LOG4QT_ROOT_PATH/include/log4qt

# 将 Log4Qt 源代码添加至项目中
include($$LOG4QT_ROOT_PATH/src/log4qt/log4qt.pri)
include($$LOG4QT_ROOT_PATH/build.pri)
include($$LOG4QT_ROOT_PATH/g++.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

RC_ICONS = sysIcon.ico
