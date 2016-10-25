#pragma once

#include "accent/accentpainter.h"

#include <QColor>

class CinemaAccentPainter : public AccentPainter
{
public:
    CinemaAccentPainter(const QColor& shadeColor, int alpha = 100);

    void paint(QPainter* painter, const RegionContext* context);
    void paint(QPainter *painter, const QRect& scope, const QRect& region);

private:
    QColor _shadeColor;
    int _alpha;
};
