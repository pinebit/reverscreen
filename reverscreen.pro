QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = REVERSCREEN
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    CV/cvmodelbuilder.cpp \
    CV/cvmodel.cpp \
    CV/cvmodelbuilderoptions.cpp \
    regionselector.cpp \
    fineselectionstrategy.cpp \
    snapselectionstrategy.cpp \
    fullscreenselectiondialog.cpp \
    fadeanimation.cpp

HEADERS  += mainwindow.h \
    CV/cvmodelbuilder.h \
    CV/cvmodel.h \
    CV/cvmodelbuilderoptions.h \
    regionselector.h \
    selectionstrategy.h \
    fineselectionstrategy.h \
    snapselectionstrategy.h \
    fullscreenselectiondialog.h \
    fadeanimation.h

RESOURCES += \
    reverscreen.qrc

INCLUDEPATH += C:/OpenCV/mingw_release/include
LIBS += -L"C:/OpenCV/mingw_release/x86/mingw/lib"
LIBS += \
    -lopencv_core2413.dll \
    -lopencv_imgproc2413.dll

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

win32:RC_ICONS += images/reverscreen.ico

include(QtAwesome/QtAwesome.pri)

SUBDIRS += \
    QtAwesome/QtAwesome.pro
