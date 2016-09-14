#include <accent/hatchingaccentpainter.h>

#include <QPainter>
#include <QBrush>


void HatchingAccentPainter::paint(QPainter *painter, QRect scope)
{
    Q_UNUSED(scope);
    Q_ASSERT(painter != NULL);

    QBrush brush(_color, Qt::BDiagPattern);

    painter->fillRect(_region, brush);
}
