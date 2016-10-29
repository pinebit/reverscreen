#pragma once

#include "accent/accentpainter.h"

#include <QColor>

class HatchingAccentPainter : public AccentPainter
{
public:
    HatchingAccentPainter(const QColor& color);

    void paint(QPainter *painter, const QRect& scope, const QRect& region);

private:
    QColor _color;
};

