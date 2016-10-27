#pragma once

#include <QRect>
#include <QPoint>
#include <QSharedPointer>

class SnapAssistant;

class RegionContext {

public:
    RegionContext();

    bool isNull() const;
    void clearAll();
    void clearRegion();

    void setSnapAssistant(const QSharedPointer<SnapAssistant>& snapAssistant);

    void setScopeRegion(const QRect& scope);
    QRect scopeRegion() const;

    void setSelectedRegion(const QRect& selectedRegion);
    QRect selectedRegion() const;
    bool hasSelectedRegion() const;

    void setHighlightedRegion(const QRect& highlightedRegion);
    QRect highlightedRegion() const;
    bool hasHighlightedRegion() const;

    void translateHighlightedRegion(int dx, int dy);
    void changeHighlightedRegion(int dx, int dy);
    void updateHighlightedRegion(int dx, int dy);

    void updateStartPoint(const QPoint& point);
    void updateEndPoint(const QPoint& point);

    static bool hasRegion(const QRect&);

protected:
    void updateHighlightedRegion(const QRect& region);


protected:
    QRect _scopeRegion;
    QRect _selectedRegion;
    QRect _highlightedRegion;

    QSharedPointer<SnapAssistant> _snapAssistant;

    static const int _minimumWidth;
    static const int _minimumHeight;
};