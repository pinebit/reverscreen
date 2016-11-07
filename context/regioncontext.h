#pragma once

#include <QRect>
#include <QPoint>
#include <QSharedPointer>

enum class RegionType {
    customRegion, highlightedRegion
};

class RegionContext {
public:
    RegionContext();

    void setRegionType(const RegionType&);

    void clearAll();
    void clearRegion();

    void setOffset(int dx, int dy);

    void setScopeRegion(const QRect& scope);
    QRect scopeRegion() const;

    void setCustomRegion(const QRect& customRegion);
    QRect customRegion(bool bNormalized = true) const;

    void setHighlightedRegion(const QRect& highlightedRegion);
    QRect highlightedRegion() const;

    QRect selectedRegion() const;
    bool hasSelectedRegion() const;

    void updateHighlightedRegion(int dx, int dy);

    static bool isValidRegion(const QRect&);

protected:
    void updateHighlightedRegion(const QRect& region);

protected:
    RegionType _regionType;

    QRect _scopeRegion;
    QRect _customRegion;
    QRect _highlightedRegion;

    QPoint _offset;

    static const int _minimumWidth;
    static const int _minimumHeight;
};
