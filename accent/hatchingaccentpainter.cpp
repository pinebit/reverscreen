#include "accent/hatchingaccentpainter.h"

#include <QPainter>
#include <QBrush>


HatchingAccentPainter::HatchingAccentPainter(const QColor& color)
    : _color(color){
}

void HatchingAccentPainter::paint(QPainter* painter, const RegionContext* context) {
    Q_ASSERT(context != NULL);
    this->paint(painter, context->scopeRegion(), context->selectedRegion());
}

void HatchingAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region) {
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    QBrush brush(_color, Qt::BDiagPattern);
    painter->fillRect(region, brush);
}
