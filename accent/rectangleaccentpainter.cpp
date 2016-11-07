#include "accent/rectangleaccentpainter.h"

#include <QPainter>

RectangleAccentPainter::RectangleAccentPainter(const QPen& pen)
    : _pen(pen)
{
}

void RectangleAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region) {
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    painter->setPen(_pen);
    painter->drawRect(region.adjusted(0, 0, -1, -1));
}

RectangleSolidLineAccentPainter::RectangleSolidLineAccentPainter()
    : RectangleAccentPainter(QPen(Qt::red, 1, Qt::SolidLine))
{
}

RectangleDashLineAccentPainter::RectangleDashLineAccentPainter()
    : RectangleAccentPainter(QPen(Qt::red, 1, Qt::DashLine))
{
}
