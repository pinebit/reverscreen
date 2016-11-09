#include "cinemaselector.h"
#include "userselection.h"

CinemaSelector::CinemaSelector(const QRect& scope)
    : _scope(scope)
{
}

QRect CinemaSelector::select(const UserSelection *selection)
{
    if (!selection->isSelected()) {
        return _scope;
    }

    return selection->getRect();
}

QPainterPath CinemaSelector::render(const QRect &selection)
{
    QPainterPath path;

    if (selection.isNull()) {
        path.addRect(_scope);
    }
    else {
        // top shader
        QRect topRect(_scope.topLeft(), QPoint(_scope.right(), selection.top() - 1));
        path.addRect(topRect);
        // bottom shader
        QRect bottomRect(QPoint(_scope.left(), selection.bottom() + 1),_scope.bottomRight());
        path.addRect(bottomRect);
        // left shader
        QRect leftRect(QPoint(_scope.left(), selection.top()), selection.bottomLeft());
        path.addRect(leftRect);
        // right shader
        QRect rightRect(selection.topRight(), QPoint(_scope.right(), selection.bottom()));
        path.addRect(rightRect);
    }

    return path;
}
