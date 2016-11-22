
INCLUDEPATH += $$PWD

win32 {
    RC_ICONS += $$PWD/reverscreen.ico
} 

unix {
    ICON = $$PWD/reverscreen.icns
}

DISTFILES += \
    $$PWD/reverscreen-icon-design.xlsx
   