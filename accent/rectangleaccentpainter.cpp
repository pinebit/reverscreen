#include <accent/rectangleaccentpainter.h>

#include <QPainter>


RectangleAccentPainter::RectangleAccentPainter(QColor color, int width)
    : _color(color)
    , _width(width)
{
}

void RectangleAccentPainter::paint(QPainter *painter, QRect scope, QRect region)
{
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    QPen pen(_color);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(_width);

    painter->setPen(pen);
    painter->drawRect(region);
}
