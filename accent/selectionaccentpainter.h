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
    SelectionAccentPainter(QColor rectColor, QColor shadeColor);

    void paint(QPainter *painter, QRect scope, QRect region);

private:
    CinemaAccentPainter _cinemaPainter;
    RectangleAccentPainter _rectanglePainter;
};
