#include "accent/selectionaccentpainter.h"

#include <QPainter>
#include <QPen>
#include <QDebug>

SelectionAccentPainter::SelectionAccentPainter(const QColor& rectColor, const QColor& shadeColor)
    : _cinemaScopePainter(shadeColor, 100)
    , _rectangleSelectedPainter(QPen(rectColor, 1, Qt::SolidLine))
{
}

void SelectionAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region) {
    Q_ASSERT(painter != NULL);

    _cinemaScopePainter.paint(painter, scope, region);
    _rectangleSelectedPainter.paint(painter, scope, region);

    drawSizeBanner(painter, region, Qt::red);
}

void SelectionAccentPainter::drawSizeBanner(QPainter *painter, const QRect &rect, QColor baseColor)
{
    int rw = rect.width();
    int rh = rect.height();
    if (rw > 1 || rh > 1) {
        QString text = QString("%1x%2").arg(rw).arg(rh);
        QRect bannerRect(rect.bottomRight() - QPoint(80, 0),
                         rect.bottomRight() + QPoint(0, 20));

        baseColor.setAlpha(150);
        QBrush bannerBrush(baseColor);
        painter->fillRect(bannerRect, bannerBrush);

        QPen textPen(Qt::white);
        painter->setPen(textPen);

        painter->drawText(bannerRect, text, Qt::AlignHCenter | Qt::AlignVCenter);
    }
}
