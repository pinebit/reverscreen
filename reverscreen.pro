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
    fullscreenselectiondialog.cpp \
    dock/colorswidget.cpp \
    dock/accentwidget.cpp \
    controls/coloractionwidget.cpp \
    awesomeservice.cpp \
    accent/rectangleaccentpainter.cpp \
    accent/cinemaaccentpainter.cpp \
    accent/hatchingaccentpainter.cpp \
    accent/selectionaccentpainter.cpp \
    assistant/cvsnapassistant.cpp \
    context/regioncontext.cpp \
    widgetutils.cpp \
    rsview.cpp

HEADERS  += mainwindow.h \
    cv/cvmodelbuilder.h \
    cv/cvmodel.h \
    cv/cvmodelbuilderoptions.h \
    fullscreenselectiondialog.h \
    dock/colorswidget.h \
    dock/accentwidget.h \
    controls/coloractionwidget.h \
    awesomeservice.h \
    accent/accentpainter.h \
    accent/rectangleaccentpainter.h \
    accent/cinemaaccentpainter.h \
    accent/hatchingaccentpainter.h \
    accent/selectionaccentpainter.h \
    assistant/snapassistant.h \
    assistant/cvsnapassistant.h \
    context/regioncontext.h \
    widgetutils.h \
    rsview.h
    
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
