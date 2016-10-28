#include "accent/rectangleaccentpainter.h"

#include <QPainter>

RectangleAccentPainter::RectangleAccentPainter(const QPen& pen)
    : _pen(pen)
{
}

void RectangleAccentPainter::paint(QPainter* painter, const RegionContext* context) {
    Q_ASSERT(context != NULL);
    this->paint(painter, context->scopeRegion(), context->selectedRegion());
}

void RectangleAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region) {
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    painter->setPen(_pen);
    painter->drawRect(region);
}
