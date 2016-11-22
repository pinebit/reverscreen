
INCLUDEPATH += $$PWD

include(3rdparty/3rdparty.pri)
include(platform/platform.pri)

SOURCES += \
    $$PWD/main.cpp\
    $$PWD/mainwindow.cpp \
    $$PWD/cv/cvmodelbuilder.cpp \
    $$PWD/cv/cvmodel.cpp \
    $$PWD/cv/cvmodelbuilderoptions.cpp \
    $$PWD/dialogs/fullscreenselectiondialog.cpp \
    $$PWD/dialogs/helpdialog.cpp \
    $$PWD/dock/colorswidget.cpp \
    $$PWD/dock/markerwidget.cpp \
    $$PWD/controls/coloractionwidget.cpp \
    $$PWD/accent/rectangleaccentpainter.cpp \
    $$PWD/accent/cinemaaccentpainter.cpp \
    $$PWD/accent/markeraccentpainter.cpp \
    $$PWD/selector/cinemaselector.cpp \
    $$PWD/selector/cvselector.cpp \
    $$PWD/selector/markerselector.cpp \
    $$PWD/services/awesomeservice.cpp \
    $$PWD/widgetutils.cpp \
    $$PWD/rsview.cpp \
    $$PWD/userselection.cpp \
    $$PWD/params.cpp

HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/cv/cvmodelbuilder.h \
    $$PWD/cv/cvmodel.h \
    $$PWD/cv/cvmodelbuilderoptions.h \
    $$PWD/dialogs/fullscreenselectiondialog.h \
    $$PWD/dialogs/helpdialog.h \
    $$PWD/dock/markerwidget.h \
    $$PWD/dock/colorswidget.h \
    $$PWD/controls/coloractionwidget.h \
    $$PWD/accent/accentpainter.h \
    $$PWD/accent/rectangleaccentpainter.h \
    $$PWD/accent/cinemaaccentpainter.h \
    $$PWD/accent/markeraccentpainter.h \
    $$PWD/selector/selector.h \
    $$PWD/selector/cinemaselector.h \
    $$PWD/selector/cvselector.h \
    $$PWD/selector/markerselector.h \
    $$PWD/services/awesomeservice.h \
    $$PWD/widgetutils.h \
    $$PWD/rsview.h \
    $$PWD/userselection.h \
    $$PWD/params.h
