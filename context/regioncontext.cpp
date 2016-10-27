
#include "context/regioncontext.h"
#include "assistant/snapassistant.h"

#include <QDebug>

const int RegionContext::_minimumWidth = 3;
const int RegionContext::_minimumHeight = 3;

RegionContext::RegionContext(){
}

bool RegionContext::isNull() const {
    return _snapAssistant.isNull();
}

void RegionContext::clearAll() {
    _snapAssistant.clear();
    _scopeRegion = _selectedRegion = _highlightedRegion = QRect();
}

void RegionContext::clearRegion() {
    _selectedRegion = _highlightedRegion = QRect();
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

bool RegionContext::hasSelectedRegion() const {
    return (_selectedRegion.width() >= _minimumWidth
         && _selectedRegion.height() >= _minimumHeight);
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

bool RegionContext::hasHighlightedRegion() const {
    return (_highlightedRegion.width() >= _minimumWidth
         && _highlightedRegion.height() >= _minimumHeight);
}

void RegionContext::translateHighlightedRegion(int dx, int dy){
    if (_highlightedRegion.isNull()){
        return;
    }

    QRect region = _highlightedRegion.translated(dx, dy);
    updateHighlightedRegion(region);
}

void RegionContext::changeHighlightedRegion(int dx, int dy){
    if (_highlightedRegion.isNull()){
        return;
    }

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
    if (_highlightedRegion.isNull()){
        return;
    }

    QRect region = _highlightedRegion.adjusted(-dx, -dy, dx, dy);
    if (region.width() <= _minimumWidth){
        region.setX(_highlightedRegion.x());
        region.setWidth(_minimumWidth);
    }
    if (region.height() <= _minimumHeight){
        region.setY(_highlightedRegion.y());
        region.setHeight(_minimumHeight);
    }

    if (_scopeRegion.intersects(region)) {
        int top = (_scopeRegion.top() < region.top())
                ? region.top() : _scopeRegion.top();
        _highlightedRegion.setTop(top);

        int left = (_scopeRegion.left() < region.left())
                ? region.left() : _scopeRegion.left();
        _highlightedRegion.setLeft(left);

        int bottom = (_scopeRegion.bottom() > region.bottom())
                ? region.bottom() : _scopeRegion.bottom()-1;
        _highlightedRegion.setBottom(bottom);

        int right = (_scopeRegion.right() > region.right())
                ? region.right() : _scopeRegion.right()-1;
        _highlightedRegion.setRight(right);
    } else {
        if (_scopeRegion.contains(region, false)) {
            _highlightedRegion = region;
        }
    }
}

void RegionContext::updateHighlightedRegion(const QRect& region){
    if (_scopeRegion.contains(region, true)) {
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

bool RegionContext::hasRegion(const QRect& region) {
    return (region.width() >= _minimumWidth && region.height() >= _minimumHeight);
}
