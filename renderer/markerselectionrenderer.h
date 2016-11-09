#pragma once

#include <renderer/selectionrenderer.h>
#include <cv/cvmodel.h>

#include <QSharedPointer>

class MarkerSelectionRenderer : public SelectionRenderer
{
public:
    MarkerSelectionRenderer(QSharedPointer<CvModel> cvModel);

    QPainterPath render(const UserSelection *selection);

private:
    QSharedPointer<CvModel> _cvModel;
};
