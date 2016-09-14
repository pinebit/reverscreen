#pragma once

#include <accent/accentpainter.h>


class CinemaAccentPainter : public AccentPainter
{
public:
    void paint(QPainter *painter, QRect scope);
};
