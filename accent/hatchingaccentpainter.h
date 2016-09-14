#pragma once

#include <accent/accentpainter.h>


class HatchingAccentPainter : public AccentPainter
{
public:
    void paint(QPainter *painter, QRect scope);
};

