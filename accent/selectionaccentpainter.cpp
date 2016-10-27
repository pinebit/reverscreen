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

    if (!context->hasSelectedRegion() && !context->hasHighlightedRegion()){
        return;
    }

    const QRect& selectedRegion = context->selectedRegion();

    _cinemaScopePainter.paint(painter, context->scopeRegion(), selectedRegion);

    const QRect& highlightedRegion = context->highlightedRegion();
    if (selectedRegion.contains(highlightedRegion, false)
     || selectedRegion.intersects(highlightedRegion) ) {

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

    if (!RegionContext::hasRegion(region)){
        return;
    }
    _cinemaScopePainter.paint(painter, scope, region);
    _rectangleSelectedPainter.paint(painter, scope, region);
}
