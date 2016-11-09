#include "markeraccentpainter.h"

#include <QBrush>
#include <QPainter>

#define DEFAULT_PEN_WIDTH 3

MarkerAccentPainter::MarkerAccentPainter(const QColor& color, bool fill)
    : _color(color)
    , _fill(fill)
{
    _color.setAlpha(150);
}

void MarkerAccentPainter::paint(QPainter *painter, const QPainterPath &path)
{
    if (_fill) {
        QBrush brush(_color);
        painter->fillPath(path, brush);
    }
    else {
        QPen pen(_color);
        pen.setWidth(DEFAULT_PEN_WIDTH);
        painter->setPen(pen);
        painter->drawPath(path);
    }
}
