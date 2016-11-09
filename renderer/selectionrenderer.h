#pragma once

#include <QPainterPath>

class UserSelection;

// Represents base abstract selection renderer object.
class SelectionRenderer
{
public:
    virtual ~SelectionRenderer() {}

    virtual QPainterPath render(const UserSelection* selection) = 0;
};
