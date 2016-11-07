#include "accent/hatchingaccentpainter.h"

#include <QPainter>
#include <QBrush>


HatchingAccentPainter::HatchingAccentPainter(const QBrush& brush)
    : _brush(brush){
}

void HatchingAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region) {
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    painter->fillRect(region, _brush);
}
