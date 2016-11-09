#include "accent/cinemaaccentpainter.h"
#include "params.h"

#include <QPainter>

CinemaAccentPainter::CinemaAccentPainter(const QColor& color)
{
    QColor transparentColor = color;
    transparentColor.setAlpha(Params::ShadeAlpha);
    _brush = QBrush(transparentColor);
}

void CinemaAccentPainter::paint(QPainter *painter, const QPainterPath &path)
{
    painter->fillPath(path, _brush);
}
