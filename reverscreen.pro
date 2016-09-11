QT += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = REVERSCREEN
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CV/cvmodelbuilder.cpp \
    CV/cvmodel.cpp \
    CV/cvmodelbuilderoptions.cpp \
    regionselector.cpp \
    fineselectionstrategy.cpp \
    snapselectionstrategy.cpp

HEADERS  += mainwindow.h \
    CV/cvmodelbuilder.h \
    CV/cvmodel.h \
    CV/cvmodelbuilderoptions.h \
    regionselector.h \
    selectionstrategy.h \
    fineselectionstrategy.h \
    snapselectionstrategy.h \
    fullscreenselectiondialog.h

FORMS    +=

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
