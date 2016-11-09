#include "accent/cinemaaccentpainter.h"

#include <QPainter>

CinemaAccentPainter::CinemaAccentPainter(const QColor& color)
{
    QColor transparentColor = color;
    transparentColor.setAlpha(150);
    _brush.setColor(transparentColor);
}

void CinemaAccentPainter::paint(QPainter *painter, const QPainterPath &path)
{
    painter->fillPath(path, _brush);
}
