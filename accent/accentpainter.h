#pragma once

#include <QRect>
#include "context/regioncontext.h"

class QPainter;

// Base class for all accent painters
class AccentPainter
{
public:
    AccentPainter() {}
    virtual ~AccentPainter() {}

    virtual void paint(QPainter* painter, const RegionContext* context) = 0;
    virtual void paint(QPainter *painter, const QRect& scope, const QRect& region) = 0;
};
