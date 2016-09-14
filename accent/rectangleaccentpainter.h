#pragma once

#include <accent/accentpainter.h>


class RectangleAccentPainter : public AccentPainter
{
public:
    void paint(QPainter *painter, QRect scope);
};
