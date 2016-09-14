#pragma once

#include <accent/accentpainter.h>

#include <QColor>


class CinemaAccentPainter : public AccentPainter
{
public:
    CinemaAccentPainter(QColor shadeColor);

    void paint(QPainter *painter, QRect scope, QRect region);

private:
    QColor _shadeColor;
};
