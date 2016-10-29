#pragma once

#include "accent/accentpainter.h"
#include "accent/cinemaaccentpainter.h"
#include "accent/rectangleaccentpainter.h"

#include <QColor>

class SelectionAccentPainter : public AccentPainter
{
public:
    SelectionAccentPainter(const QColor& rectColor, const QColor& shadeColor);

    void paint(QPainter *painter, const QRect& scope, const QRect& region);

private:
    void drawSizeBanner(QPainter *painter, const QRect& rect, QColor baseColor);

    CinemaAccentPainter _cinemaScopePainter;
    RectangleAccentPainter _rectangleSelectedPainter;
};
