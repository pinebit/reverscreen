#include <accent/cinemaaccentpainter.h>

#include <QPainter>


void CinemaAccentPainter::paint(QPainter *painter, QRect scope)
{
    Q_ASSERT(painter != NULL);

    QColor color(_color);
    color.setAlpha(100);

    QBrush brush(color);

    int min_x = _region.left();
    int min_y = _region.top();
    int max_x = _region.right();
    int max_y = _region.bottom();

    // top shader
    painter->fillRect(scope.left(), scope.top(), scope.width(), min_y, brush);

    // bottom shader
    painter->fillRect(scope.left(), max_y + 1, scope.width(), scope.height() - max_y, brush);

    // left shader
    painter->fillRect(scope.left(), min_y, min_x, max_y - min_y + 1, brush);

    // right shader
    painter->fillRect(max_x + 1, min_y, scope.width() - max_x, max_y - min_y + 1, brush);
}
