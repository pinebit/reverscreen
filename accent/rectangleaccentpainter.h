#pragma once

#include "accent/accentpainter.h"

#include <QColor>

class RectangleAccentPainter : public AccentPainter
{
public:
    RectangleAccentPainter(const QColor& color, int width = 1);

    void paint(QPainter* painter, const RegionContext* context);
    void paint(QPainter* painter, const QRect& scope, const QRect& region);

private:
    QColor _color;
    int _width;
};
