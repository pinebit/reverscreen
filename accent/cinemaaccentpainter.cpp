#include "accent/cinemaaccentpainter.h"

#include <QPainter>
#include <QtGlobal>
#include <QDebug>

CinemaAccentPainter::CinemaAccentPainter(const QBrush& brush)
    : _brush(brush) {
}

void CinemaAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region){
    Q_ASSERT(painter != NULL);

    if (region.height() == 0 || region.width() == 0) {
        painter->fillRect(scope, _brush);
        return;
    }

    // top shader
    QRect topRect(scope.topLeft(), QPoint(scope.right(), region.top() - 1));
    painter->fillRect(topRect, _brush);
    // bottom shader
    QRect bottomRect(QPoint(scope.left(), region.bottom() + 1),scope.bottomRight());
    painter->fillRect(bottomRect, _brush);
    // left shader
    QRect leftRect(QPoint(scope.left(), region.top()), region.bottomLeft());
    painter->fillRect(leftRect, _brush);
    // right shader
    QRect rightRect(region.topRight(), QPoint(scope.right(), region.bottom()));
    painter->fillRect(rightRect, _brush);
}


void CinemaAccentPainter::paint(QPainter *painter, const QPainterPath &path)
{
    // TODO:
}
