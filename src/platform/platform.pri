win32 {
    include(windows/windows.pri)
}
unix:!macx {
    include(generic/generic.pri)
}
macx {
    include(generic/generic.pri)
}
