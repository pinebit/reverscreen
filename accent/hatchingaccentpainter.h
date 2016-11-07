#pragma once

#include "accent/accentpainter.h"

#include <QBrush>

class HatchingAccentPainter : public AccentPainter
{
public:
    HatchingAccentPainter(const QBrush& brush);

    void paint(QPainter *painter, const QRect& scope, const QRect& region);

private:
    QBrush _brush;
};

