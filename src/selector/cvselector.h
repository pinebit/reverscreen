#pragma once

#include <selector/selector.h>
#include <cv/cvmodel.h>

#include <QSharedPointer>

class CvSelector : public Selector
{
public:
    CvSelector(QSharedPointer<CvModel> cvModel);

    QRect select(const UserSelection *selection);
    QPainterPath render(const QRect &selection);

private:
    QSharedPointer<CvModel> _cvModel;
};
