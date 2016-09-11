#pragma once

#include <QSharedPointer>

#include <selectionstrategy.h>

class CvModel;


// This strategy uses CvModel data.
class SnapSelectionStrategy : public SelectionStrategy
{
public:
    SnapSelectionStrategy(QSharedPointer<CvModel> model);

    QRect hint(const QPoint &point);
    QPoint begin(const QPoint &point);
    QPoint end(const QRect &rect);
    QRect adjust(const QRect &rect);

private:
    QSharedPointer<CvModel> _model;
};
