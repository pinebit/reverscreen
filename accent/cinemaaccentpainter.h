#pragma once

#include "accent/accentpainter.h"

#include <QBrush>

class CinemaAccentPainter : public AccentPainter
{
public:
    CinemaAccentPainter(const QBrush& brush);

    void paint(QPainter *painter, const QRect& scope, const QRect& region);

private:
    QBrush _brush;
};
