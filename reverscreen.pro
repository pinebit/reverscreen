QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = REVERSCREEN
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    cv/cvmodelbuilder.cpp \
    cv/cvmodel.cpp \
    cv/cvmodelbuilderoptions.cpp \
    regionselector.cpp \
    fineselectionstrategy.cpp \
    snapselectionstrategy.cpp \
    fullscreenselectiondialog.cpp \
    fadeanimation.cpp \
    dock/colorswidget.cpp \
    widgetfactory.cpp \
    dock/accentwidget.cpp

HEADERS  += mainwindow.h \
    cv/cvmodelbuilder.h \
    cv/cvmodel.h \
    cv/cvmodelbuilderoptions.h \
    regionselector.h \
    selectionstrategy.h \
    fineselectionstrategy.h \
    snapselectionstrategy.h \
    fullscreenselectiondialog.h \
    fadeanimation.h \
    dock/colorswidget.h \
    widgetfactory.h \
    dock/accentwidget.h

RESOURCES += \
    reverscreen.qrc

CONFIG(release, debug|release)
{
INCLUDEPATH += C:/Opencv/mingw_release/include
LIBS += -L"C:/Opencv/mingw_release/x86/mingw/lib"
}

CONFIG(debug, debug|release)
{
INCLUDEPATH += C:/Opencv/mingw_debug/include
LIBS += -L"C:/Opencv/mingw_debug/x86/mingw/lib"
}

LIBS += \
    -lopencv_core2413.dll \
    -lopencv_imgproc2413.dll

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

win32:RC_ICONS += images/reverscreen.ico

include(QtAwesome/QtAwesome.pri)

SUBDIRS += \
    QtAwesome/QtAwesome.pro \
    QtAwesome/QtAwesome.pro
