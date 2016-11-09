#include "markerselectionrenderer.h"
#include "userselection.h"

#define ROUND_CORNER_SIZE   8

MarkerSelectionRenderer::MarkerSelectionRenderer(QSharedPointer<CvModel> cvModel)
    : _cvModel(cvModel)
{
}

QPainterPath MarkerSelectionRenderer::render(const UserSelection *selection)
{
    QPainterPath path;

    if (selection->isSelected()) {
        QPolygon polygon = _cvModel->getConvexHull(selection->track());
        QRect brect = polygon.boundingRect();
        brect.adjust(-ROUND_CORNER_SIZE, -ROUND_CORNER_SIZE, ROUND_CORNER_SIZE, ROUND_CORNER_SIZE);
        path.addRoundedRect(brect, ROUND_CORNER_SIZE, ROUND_CORNER_SIZE);
    }

    return path;
}
