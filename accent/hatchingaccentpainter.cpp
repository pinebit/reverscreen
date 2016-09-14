#include <accent/hatchingaccentpainter.h>

#include <QPainter>
#include <QBrush>


HatchingAccentPainter::HatchingAccentPainter(QColor color)
    : _color(color)
{
}

void HatchingAccentPainter::paint(QPainter *painter, QRect scope, QRect region)
{
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    QBrush brush(_color, Qt::BDiagPattern);

    painter->fillRect(region, brush);
}
