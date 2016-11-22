QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = REVERSCREEN
TEMPLATE = app

include(src/src.pri)
include(icons/icons.pri)

RESOURCES += \
    reverscreen.qrc

DISTFILES += \
    website/img/reverscreen-demo.gif \
    website/img/reverscreen-64.png \
    website/img/rs-step-1.png \
    website/img/rs-step-2.PNG \
    website/img/rs-step-3.PNG \
    website/img/rs-step-4.PNG \
    website/favicon.ico \
    website/css/animate.css \
    website/css/bootstrap.min.css \
    website/css/styles.css \
    website/index.html \
    nsis/reverscreen.nsi \
    new-rs-icon.png \
    .gitignore \
    BUILDING \
    LICENSE \
    README.md

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
}

unix {
INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib
LIBS += -lopencv_core -lopencv_imgproc -lz
}

osx {
QMAKE_INFO_PLIST = Info.plist
}

QMAKE_CXXFLAGS += -Wno-deprecated-declarations
