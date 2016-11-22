#pragma once

#include <selector/selector.h>
#include <cv/cvmodel.h>

#include <QSharedPointer>

class MarkerSelector : public Selector
{
public:
    MarkerSelector(QSharedPointer<CvModel> cvModel);

    QRect select(const UserSelection *selection);
    QPainterPath render(const QRect &selection);

private:
    QSharedPointer<CvModel> _cvModel;
};
