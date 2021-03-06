#pragma once

#include "accent/accentpainter.h"

#include <QColor>

class MarkerAccentPainter : public AccentPainter
{
public:
    MarkerAccentPainter(const QColor& color, bool fill);

    void paint(QPainter *painter, const QPainterPath &path);

private:
    QColor _color;
    bool _fill;
};
