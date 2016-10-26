#pragma once

#include "accent/accentpainter.h"
#include "accent/cinemaaccentpainter.h"
#include "accent/rectangleaccentpainter.h"

#include <QColor>

class CinemaAccentPainter;
class RectangleAccentPainter;

class SelectionAccentPainter : public AccentPainter
{
public:
    SelectionAccentPainter(const QColor& rectColor, const QColor& shadeColor);

    void paint(QPainter* painter, const RegionContext* context);
    void paint(QPainter *painter, const QRect& scope, const QRect& region);

private:
    CinemaAccentPainter _cinemaScopePainter;
    CinemaAccentPainter _cinemaSelectedPainter;
    RectangleAccentPainter _rectangleSelectedPainter;
    RectangleAccentPainter _rectangleHighlightedPainter;
};
