QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = REVERSCREEN
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagecropwidget.cpp \
    linesdetector.cpp \
    CV/cvmodelbuilder.cpp \
    CV/cvmodel.cpp \
    CV/cvmodelbuilderoptions.cpp

HEADERS  += mainwindow.h \
    imagecropwidget.h \
    asmopencv.h \
    linesdetector.h \
    CV/cvmodelbuilder.h \
    CV/cvmodel.h \
    CV/cvmodelbuilderoptions.h

FORMS    += mainwindow.ui

RESOURCES += \
    reverscreen.qrc

INCLUDEPATH += C:/OpenCV/mingw/install/include
LIBS += -L"C:/OpenCV/mingw/install/x86/mingw/lib" \
    -lopencv_core2413 \
    -lopencv_imgproc2413 \
    -lopencv_highgui2413

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

win32:RC_ICONS += images/reverscreen.ico

include(QtAwesome/QtAwesome.pri)

SUBDIRS += \
    QtAwesome/QtAwesome.pro

DISTFILES += \
    images/copy.png \
    images/new.png \
    images/save.png \
    website/css/animate.css \
    website/css/styles.css \
    website/index.html \
    README.md
