#include "cinemaselectionrenderer.h"
#include "userselection.h"

CinemaSelectionRenderer::CinemaSelectionRenderer(const QRect& scope)
    : _scope(scope)
{
}

QPainterPath CinemaSelectionRenderer::render(const UserSelection *selection)
{
    QPainterPath path;

    if (!selection->isSelected()) {
        path.addRect(_scope);
    }
    else {
        QRect region = selection->getRect();

        // top shader
        QRect topRect(_scope.topLeft(), QPoint(_scope.right(), region.top() - 1));
        path.addRect(topRect);
        // bottom shader
        QRect bottomRect(QPoint(_scope.left(), region.bottom() + 1),_scope.bottomRight());
        path.addRect(bottomRect);
        // left shader
        QRect leftRect(QPoint(_scope.left(), region.top()), region.bottomLeft());
        path.addRect(leftRect);
        // right shader
        QRect rightRect(region.topRight(), QPoint(_scope.right(), region.bottom()));
        path.addRect(rightRect);
    }

    return path;
}
