#pragma once

#include "accent/accentpainter.h"

#include <QPen>

class RectangleAccentPainter : public AccentPainter
{
public:
    RectangleAccentPainter(const QPen& pen);

    void paint(QPainter *painter, const QPainterPath &path);

private:
    QPen _pen;
};
