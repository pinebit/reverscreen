#include <fineselectionstrategy.h>

FineSelectionStrategy::FineSelectionStrategy()
{
}

QRect FineSelectionStrategy::hint(const QPoint &point)
{
    return QRect(point, QSize(1, 1));
}

QPoint FineSelectionStrategy::begin(const QPoint &point)
{
    return point;
}

QPoint FineSelectionStrategy::end(const QRect &rect)
{
    return rect.bottomRight();
}

QRect FineSelectionStrategy::adjust(const QRect &rect)
{
    return rect;
}
