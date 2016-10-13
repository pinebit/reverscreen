#pragma once

#include "accent/accentpainter.h"

#include <QColor>


class HatchingAccentPainter : public AccentPainter
{
public:
    HatchingAccentPainter(QColor color);

    void paint(QPainter *painter, QRect scope, QRect region);

private:
    QColor _color;
};

