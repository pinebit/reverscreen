#include <accent/selectionaccentpainter.h>

#include <QPainter>


SelectionAccentPainter::SelectionAccentPainter(QColor rectColor, QColor shadeColor)
    : _cinemaPainter(shadeColor)
    , _rectanglePainter(rectColor, 1)
{
}

void SelectionAccentPainter::paint(QPainter *painter, QRect scope, QRect region)
{
    Q_ASSERT(painter != NULL);

    _cinemaPainter.paint(painter, scope, region);
    _rectanglePainter.paint(painter, scope, region);
}
