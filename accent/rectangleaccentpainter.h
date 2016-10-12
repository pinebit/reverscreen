#pragma once

#include "accent/accentpainter.h"

#include <QColor>


class RectangleAccentPainter : public AccentPainter
{
public:
    RectangleAccentPainter(QColor color, int width = 1);

    void paint(QPainter *painter, QRect scope, QRect region);

private:
    QColor _color;
    int _width;
};
