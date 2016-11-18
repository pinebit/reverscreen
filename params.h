#pragma once

#include <QColor>

struct Params
{
    static QColor ShadeColor;
    static int    ShadeAlpha;
    static QColor SelectionAccentColor;
    static int    MarkerFrameWidth;
    static int    MarkerAlpha;
    static int    MarkerRoundCornerSize;
    static int    SelectionMarginSize;

private:
    Params() {}
};
