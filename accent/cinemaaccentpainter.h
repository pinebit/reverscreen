#pragma once

#include "accent/accentpainter.h"

#include <QBrush>

class CinemaAccentPainter : public AccentPainter
{
public:
    CinemaAccentPainter(const QColor& color);

    void paint(QPainter *painter, const QPainterPath &path);

private:
    QBrush _brush;
};
