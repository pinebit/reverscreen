#pragma once

#include <QImage>
#include <QWidget>

#ifdef Q_OS_WIN

#include <windows.h>
#include <wingdi.h>

HBITMAP convertQImageToHBITMAP(const QImage& image);
HBITMAP captureQWidget(const QWidget& widget);

#endif

