#pragma once

#include <renderer/selectionrenderer.h>
#include <cv/cvmodel.h>

#include <QSharedPointer>

class CvSelectionRenderer : public SelectionRenderer
{
public:
    CvSelectionRenderer(QSharedPointer<CvModel> cvModel);

    QPainterPath render(const UserSelection *selection);

private:
    QSharedPointer<CvModel> _cvModel;
};
