#include "accent/selectionaccentpainter.h"

#include <QPainter>
#include <QDebug>

SelectionAccentPainter::SelectionAccentPainter(const QColor& rectColor, const QColor& shadeColor)
    : _cinemaScopePainter(shadeColor, 100)
    , _cinemaSelectedPainter(Qt::red, 50)
    , _rectangleSelectedPainter(Qt::red)
    , _rectangleHighlightedPainter(Qt::yellow){
    Q_UNUSED(rectColor);
}

void SelectionAccentPainter::paint(QPainter *painter, const RegionContext* context) {
    Q_ASSERT(painter != NULL);
    Q_ASSERT(context != NULL);

    const QRect& selectedRegion = context->selectedRegion();
    const QRect& highlightedRegion = context->highlightedRegion();

    _cinemaScopePainter.paint(painter, context->scopeRegion(), selectedRegion);

    if (selectedRegion.contains(highlightedRegion, false)) {

        QRect intersectedRegion = (selectedRegion.intersects(highlightedRegion))
                ? selectedRegion.intersected(highlightedRegion)
                : highlightedRegion;

        _cinemaSelectedPainter.paint(painter, selectedRegion, intersectedRegion);
        _rectangleHighlightedPainter.paint(painter, selectedRegion, intersectedRegion);
    } else {
        _rectangleSelectedPainter.paint(painter, context->scopeRegion(), selectedRegion);
    }
}

void SelectionAccentPainter::paint(QPainter *painter, const QRect& scope, const QRect& region) {
    Q_ASSERT(painter != NULL);

    if (region.isNull()){
        return;
    }
    _cinemaScopePainter.paint(painter, scope, region);
    _rectangleSelectedPainter.paint(painter, scope, region);
}
