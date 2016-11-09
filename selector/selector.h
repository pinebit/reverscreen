#pragma once

#include <QPainterPath>

class UserSelection;

// Represents abstract selection and rendering logic.
class Selector
{
public:
    virtual ~Selector() {}

    virtual QRect select(const UserSelection* selection) = 0;
    virtual QPainterPath render(const QRect& selection) = 0;
};
