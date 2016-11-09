QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = REVERSCREEN
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    cv/cvmodelbuilder.cpp \
    cv/cvmodel.cpp \
    cv/cvmodelbuilderoptions.cpp \
    fullscreenselectiondialog.cpp \
    dock/colorswidget.cpp \
    controls/coloractionwidget.cpp \
    awesomeservice.cpp \
    accent/rectangleaccentpainter.cpp \
    accent/cinemaaccentpainter.cpp \
    accent/selectionaccentpainter.cpp \
    assistant/cvsnapassistant.cpp \
    context/regioncontext.cpp \
    widgetutils.cpp \
    rsview.cpp \
    dock/markerwidget.cpp \
    accent/markeraccentpainter.cpp \
    userselection.cpp

HEADERS  += mainwindow.h \
    cv/cvmodelbuilder.h \
    cv/cvmodel.h \
    cv/cvmodelbuilderoptions.h \
    fullscreenselectiondialog.h \
    dock/colorswidget.h \
    controls/coloractionwidget.h \
    awesomeservice.h \
    accent/accentpainter.h \
    accent/rectangleaccentpainter.h \
    accent/cinemaaccentpainter.h \
    accent/selectionaccentpainter.h \
    assistant/snapassistant.h \
    assistant/cvsnapassistant.h \
    context/regioncontext.h \
    widgetutils.h \
    rsview.h \
    dock/markerwidget.h \
    accent/markeraccentpainter.h \
    userselection.h
    
RESOURCES += \
    reverscreen.qrc

win32 {
CONFIG(release, debug|release)
{
OPENCV_PATH = C:/Opencv/mingw_release
}
CONFIG(debug, debug|release)
{
OPENCV_PATH = C:/Opencv/mingw_debug
}

INCLUDEPATH += $$OPENCV_PATH/include
LIBS += -L"$$OPENCV_PATH/x86/mingw/lib"
LIBS += -lopencv_core2413.dll -lopencv_imgproc2413.dll

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

RC_ICONS += icons/reverscreen.ico
}

unix {
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib
LIBS += -lopencv_core -lopencv_imgproc -lz
ICON = icons/reverscreen.icns
}

osx {
QMAKE_INFO_PLIST = Info.plist
}

QMAKE_CXXFLAGS += -Wno-deprecated-declarations

include(QtAwesome/QtAwesome.pri)

SUBDIRS += QtAwesome/QtAwesome.pro
