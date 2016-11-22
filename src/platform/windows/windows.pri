CONFIG  += static staticlib c++11
LIBS    += -luser32

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/image.h \
    $$PWD/clipboard.h

SOURCES += \
    $$PWD/image.cpp \
    $$PWD/clipboard.cpp
