#pragma once

#include <renderer/selectionrenderer.h>

#include <QSharedPointer>
#include <QRect>

class CinemaSelectionRenderer : public SelectionRenderer
{
public:
    CinemaSelectionRenderer(const QRect& scope);

    QPainterPath render(const UserSelection *selection);

private:
    QRect _scope;
};
