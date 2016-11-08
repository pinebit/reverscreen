
#include "context/regioncontext.h"
#include "assistant/snapassistant.h"

#include <QDebug>

const int RegionContext::_minimumWidth = 4;
const int RegionContext::_minimumHeight = 4;

RegionContext::RegionContext()
    : _regionType(RegionType::highlightedRegion)
    , _offset(0,0) {
}

void RegionContext::setRegionType(const RegionType& regionType){
    _regionType = regionType;
}

const RegionType& RegionContext::regionType() const{
    return _regionType;
}

void RegionContext::clearAll() {
    _scopeRegion = _customRegion = _highlightedRegion = QRect();
}

void RegionContext::clearRegion() {
    _customRegion = _highlightedRegion = QRect();
}

void RegionContext::setScopeRegion(const QRect& scope){
    _scopeRegion = scope;
}

QRect RegionContext::scopeRegion() const {
    return _scopeRegion;
}

void RegionContext::setCustomRegion(const QRect& customRegion){
    _customRegion = customRegion;
}

QRect RegionContext::customRegion(bool bNormalized) const {
    if (bNormalized) {
        return _customRegion.normalized();
    }
    return _customRegion;
}

void RegionContext::setHighlightedRegion(const QRect& highlightedRegion){
    _highlightedRegion = highlightedRegion;
}

QRect RegionContext::highlightedRegion() const {
    QRect normalizedCustomRegion(customRegion());
    QRect region(_highlightedRegion.normalized());
    region.adjust(-_offset.x(), -_offset.y(),
                  _offset.x(), _offset.y());
    if (normalizedCustomRegion.contains(region, false)) {
        return ((normalizedCustomRegion.intersects(region))
                ? normalizedCustomRegion.intersected(region)
                : region);
    }
    return region;
}

QRect RegionContext::selectedRegion() const{
    if (_regionType == RegionType::highlightedRegion) {
        return highlightedRegion();
    }
    return customRegion();
}

bool RegionContext::hasSelectedRegion() const {
    return (selectedRegion().width() >= _minimumWidth
         && selectedRegion().height() >= _minimumHeight);
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

bool RegionContext::isValidRegion(const QRect& region) {
    return (region.width() > 1 && region.height() > 1);
}

void RegionContext::setOffset(int dx, int dy){
    // correction offset
    QPoint offset(_offset + QPoint(dx, dy));
    QRect normalizedHighlightedRegion(_highlightedRegion.normalized());
    QRect region = normalizedHighlightedRegion.adjusted(-offset.x(), -offset.y(),
                                                        offset.x(), offset.y());
    if (normalizedHighlightedRegion.contains(region, true)){
        _offset = QPoint(0,0);
        return;
    }
    QRect normalizedCustomRegion(customRegion());
    if (normalizedCustomRegion.contains(region, false)){
        _offset = offset;
    }
}
