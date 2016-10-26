
#include "context/regioncontext.h"
#include "assistant/snapassistant.h"

const int RegionContext::_minimumWidth = 3;
const int RegionContext::_minimumHeight = 3;

RegionContext::RegionContext(){
}

bool RegionContext::isNull() const {
    return _snapAssistant.isNull();
}

void RegionContext::clear() {
    _snapAssistant.clear();
    _scopeRegion = _selectedRegion = _highlightedRegion = QRect();
}

void RegionContext::setSnapAssistant(const QSharedPointer<SnapAssistant>& snapAssistant) {
    _snapAssistant = snapAssistant;
}

void RegionContext::setScopeRegion(const QRect& scope){
    _scopeRegion = scope;
}

QRect RegionContext::scopeRegion() const {
    return _scopeRegion;
}

void RegionContext::setSelectedRegion(const QRect& selectedRegion){
    _selectedRegion = selectedRegion;
}

QRect RegionContext::selectedRegion() const {
    return _selectedRegion.normalized();
}

void RegionContext::setHighlightedRegion(const QRect& highlightedRegion){
    _highlightedRegion = highlightedRegion;
}

QRect RegionContext::highlightedRegion() const {
    QRect region(_highlightedRegion.normalized());
    if (_selectedRegion.contains(region, false)) {
        return ((_selectedRegion.intersects(region))
                ? _selectedRegion.intersected(region)
                : region);
    }
    return region;
}

void RegionContext::translateHighlightedRegion(int dx, int dy){
    QRect region = _highlightedRegion.translated(dx, dy);
    updateHighlightedRegion(region);
}

void RegionContext::changeHighlightedRegion(int dx, int dy){
    QRect region(_highlightedRegion);
    int width = region.width() + dx;
    int height = region.height() + dy;
    if (width >= _minimumWidth){
        region.setWidth(width);
    }
    if (height >= _minimumHeight){
        region.setHeight(height);
    }
    updateHighlightedRegion(region);
}

void RegionContext::updateHighlightedRegion(int dx, int dy){
    QPoint topLeft = _highlightedRegion.topLeft();
    QPoint bottomRight = _highlightedRegion.bottomRight();
    topLeft -= QPoint(dx, dy);
    bottomRight += QPoint(dx, dy);

    QRect region(topLeft, bottomRight);
    updateHighlightedRegion(region);
}

void RegionContext::updateHighlightedRegion(const QRect& region){
    if (_scopeRegion.contains(region, false)) {
        QRect intersectedRegion = (_scopeRegion.intersects(region))
                ? _scopeRegion.intersected(region)
                : region;
        _highlightedRegion = intersectedRegion;
    }
}

void RegionContext::updateStartPoint(const QPoint& point){
    _selectedRegion.setTopLeft(point);
    _selectedRegion.setBottomRight(point);

    QPoint snapBeginPoint = _snapAssistant->begin(point);
    _highlightedRegion.setTopLeft(snapBeginPoint);
    _highlightedRegion.setBottomRight(snapBeginPoint);
}

void RegionContext::updateEndPoint(const QPoint& point){
    _selectedRegion.setBottomRight(point);

    QPoint snapEndPoint = _snapAssistant->end(QRect(_highlightedRegion.topLeft(), point));
    _highlightedRegion.setBottomRight(snapEndPoint);
}
