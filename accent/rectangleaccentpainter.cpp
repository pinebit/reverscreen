#include <accent/rectangleaccentpainter.h>

#include <QPainter>


void RectangleAccentPainter::paint(QPainter *painter, QRect scope)
{
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    QPen pen(_color);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(4);

    painter->setPen(pen);
    painter->drawRect(_region);
}
