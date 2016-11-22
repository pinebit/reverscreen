#include "markeraccentpainter.h"
#include "params.h"

#include <QBrush>
#include <QPainter>

MarkerAccentPainter::MarkerAccentPainter(const QColor& color, bool fill)
    : _color(color)
    , _fill(fill)
{
    _color.setAlpha(Params::MarkerAlpha);
}

void MarkerAccentPainter::paint(QPainter *painter, const QPainterPath &path)
{
    if (_fill) {
        QBrush brush(_color);
        painter->fillPath(path, brush);
    }
    else {
        QColor opaque(_color);
        opaque.setAlpha(255);
        QPen pen(opaque);
        pen.setWidth(Params::MarkerFrameWidth);
        painter->setPen(pen);
        painter->drawPath(path);
    }
}
