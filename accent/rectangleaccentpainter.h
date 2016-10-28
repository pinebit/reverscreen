#pragma once

#include "accent/accentpainter.h"

#include <QPen>

class RectangleAccentPainter : public AccentPainter
{
public:
    RectangleAccentPainter(const QPen& pen);

    void paint(QPainter* painter, const RegionContext* context);
    void paint(QPainter* painter, const QRect& scope, const QRect& region);

private:
    QPen _pen;
};
