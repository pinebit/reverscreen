#pragma once

#include "accent/accentpainter.h"

#include <QColor>

class MarkerAccentPainter : public AccentPainter
{
public:
    MarkerAccentPainter(const QColor& color);

    void paint(QPainter *painter, const QRect& scope, const QRect& region);

private:
    QColor _color;
};
