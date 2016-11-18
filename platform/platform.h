#pragma once

#ifdef Q_OS_WIN
    #include <platform/windows/clipboard.h>
#else
    #include <platform/generic/clipboard.h>
#endif
