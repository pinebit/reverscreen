#include "accent/rectangleaccentpainter.h"

#include <QPainter>

RectangleAccentPainter::RectangleAccentPainter(const QColor& color, int width)
    : _color(color)
    , _width(width) {
}

void RectangleAccentPainter::paint(QPainter* painter, const RegionContext* context) {
    Q_ASSERT(context != NULL);
    this->paint(painter, context->scopeRegion(), context->selectedRegion());
}

void RectangleAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region) {
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    QPen pen(_color);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(_width);

    painter->setPen(pen);
    painter->drawRect(region);
}
