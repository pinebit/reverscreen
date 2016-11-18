#include "accent/rectangleaccentpainter.h"

#include <QPainter>

RectangleAccentPainter::RectangleAccentPainter(const QPen& pen)
    : _pen(pen)
{
}

void RectangleAccentPainter::paint(QPainter *painter, const QPainterPath &path)
{
    painter->setPen(_pen);
    painter->drawPath(path);
}
