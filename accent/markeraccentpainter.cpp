#include "markeraccentpainter.h"

#include <QBrush>
#include <QPainter>

MarkerAccentPainter::MarkerAccentPainter(const QColor& color)
    : _color(color)
{
    _color.setAlpha(100);
}

void MarkerAccentPainter::paint(QPainter *painter, const QRect &scope, const QRect &region)
{
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    QBrush brush(_color);
    painter->fillRect(region, brush);
}
