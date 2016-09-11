#pragma once

#include <selectionstrategy.h>


// Presize pixel selection strategy.
class FineSelectionStrategy : public SelectionStrategy
{
public:
    FineSelectionStrategy();

    QRect hint(const QPoint &point);
    QPoint begin(const QPoint &point);
    QPoint end(const QRect &rect);
    QRect adjust(const QRect &rect);
};
