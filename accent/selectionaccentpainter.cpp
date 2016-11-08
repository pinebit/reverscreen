#include "accent/selectionaccentpainter.h"

#include <QPainter>
#include <QPen>
#include <QDebug>

static const QColor RegionColor = Qt::red;
static const QColor ShaderColor = QColor::fromRgba(0x50a0a0a0);
static const QPen RegionSolidLinePen(RegionColor, 1, Qt::SolidLine);
static const QPen RegionDashLinePen(RegionColor, 1, Qt::DashLine);

SelectionAccentPainter::SelectionAccentPainter(const QPen& pen, const QBrush& brush)
    : _cinemaScopePainter(brush)
    , _rectangleSelectedPainter(pen)
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

SelectionSolidLineAccentPainter::SelectionSolidLineAccentPainter()
    : SelectionAccentPainter(RegionSolidLinePen,
                             QBrush(QColor(ShaderColor.red(), ShaderColor.green(), ShaderColor.blue(), 100))){
}

SelectionDashLineAccentPainter::SelectionDashLineAccentPainter()
    : SelectionAccentPainter(RegionDashLinePen,
                             QBrush(QColor(ShaderColor.red(), ShaderColor.green(), ShaderColor.blue(), 100))){
}
