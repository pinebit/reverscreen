QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = REVERSCREEN
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    cv/cvmodelbuilder.cpp \
    cv/cvmodel.cpp \
    cv/cvmodelbuilderoptions.cpp \
    dialogs/fullscreenselectiondialog.cpp \
    dialogs/helpdialog.cpp \
    dock/colorswidget.cpp \
    dock/markerwidget.cpp \
    controls/coloractionwidget.cpp \
    accent/rectangleaccentpainter.cpp \
    accent/cinemaaccentpainter.cpp \
    accent/markeraccentpainter.cpp \
    selector/cinemaselector.cpp \
    selector/cvselector.cpp \
    selector/markerselector.cpp \
    awesomeservice.cpp \
    widgetutils.cpp \
    rsview.cpp \
    userselection.cpp \
    params.cpp

HEADERS  += mainwindow.h \
    cv/cvmodelbuilder.h \
    cv/cvmodel.h \
    cv/cvmodelbuilderoptions.h \
    dialogs/fullscreenselectiondialog.h \
    dialogs/helpdialog.h \
    dock/markerwidget.h \
    dock/colorswidget.h \
    controls/coloractionwidget.h \
    accent/accentpainter.h \
    accent/rectangleaccentpainter.h \
    accent/cinemaaccentpainter.h \
    accent/markeraccentpainter.h \
    selector/selector.h \
    selector/cinemaselector.h \
    selector/cvselector.h \
    selector/markerselector.h \
    awesomeservice.h \
    widgetutils.h \
    rsview.h \
    userselection.h \
    params.h
    
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
LIBS += -lgdi32

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

include(platform/platform.pri)
include(QtAwesome/QtAwesome.pri)

SUBDIRS += QtAwesome/QtAwesome.pro \
    QtAwesome/QtAwesome.pro

DISTFILES += \
    icons/reverscreen-icon-design.xlsx \
    website/img/reverscreen-demo.gif \
    website/img/reverscreen-64.png \
    website/img/rs-step-1.png \
    website/img/rs-step-2.PNG \
    website/img/rs-step-3.PNG \
    website/img/rs-step-4.PNG \
    new-rs-icon.png \
    website/favicon.ico \
    website/css/animate.css \
    website/css/bootstrap.min.css \
    website/css/styles.css \
    website/index.html \
    .gitignore \
    BUILDING \
    LICENSE \
    README.md \
    nsis/reverscreen.nsi
    