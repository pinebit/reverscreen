#pragma once

#include <selector/selector.h>

#include <QSharedPointer>
#include <QRect>

class CinemaSelector : public Selector
{
public:
    CinemaSelector(const QRect& scope);

    QRect select(const UserSelection *selection);
    QPainterPath render(const QRect &selection);

private:
    QRect _scope;
};
