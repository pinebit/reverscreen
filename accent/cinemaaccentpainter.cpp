#include "accent/cinemaaccentpainter.h"

#include <QPainter>
#include <QtGlobal>

CinemaAccentPainter::CinemaAccentPainter(const QColor& shadeColor, int alpha)
    : _shadeColor(shadeColor)
    , _alpha(alpha) {
}

void CinemaAccentPainter::paint(QPainter *painter, const RegionContext* context) {
    Q_ASSERT(context != NULL);
    this->paint(painter, context->scopeRegion(), context->selectedRegion());
}

void CinemaAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region){
    Q_ASSERT(painter != NULL);

    QColor color(_shadeColor);
    color.setAlpha(_alpha);

    QBrush brush(color);

    if (!RegionContext::isValidRegion(region)){
        // full shader
        painter->fillRect(scope, brush);
        return;
    }

    // top shader
    QRect topRect(scope.topLeft(), QPoint(scope.right(), region.top() - 1));
    painter->fillRect(topRect, brush);
    // left shader
    QRect leftRect(QPoint(scope.left(), region.top()), region.bottomLeft());
    painter->fillRect(leftRect, brush);
    // right shader
    QRect rightRect(region.topRight(), QPoint(scope.right(), region.bottom()));
    painter->fillRect(rightRect, brush);
    // bottom shader
    QRect bottomRect(QPoint(scope.left(), region.bottom() + 1),scope.bottomRight());
    painter->fillRect(bottomRect, brush);
}
