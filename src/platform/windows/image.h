#pragma once

#include <QImage>
#include <QWidget>

#include <windows.h>
#include <wingdi.h>

class image
{
public:
    static HBITMAP convertQImageToHBITMAP(const QImage& image);
    static HBITMAP captureQWidget(const QWidget& widget);
};
