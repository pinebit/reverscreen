#include "markerselector.h"
#include "userselection.h"
#include "params.h"

MarkerSelector::MarkerSelector(QSharedPointer<CvModel> cvModel)
    : _cvModel(cvModel)
{
}

QRect MarkerSelector::select(const UserSelection *selection)
{
    if (selection->isSelected()) {
        QPolygon polygon = _cvModel->getConvexHull(selection->track());
        return polygon.boundingRect();
    }

    return QRect();
}

QPainterPath MarkerSelector::render(const QRect &selection)
{
    QPainterPath path;

    if (!selection.isNull()) {
        path.addRoundedRect(selection, Params::MarkerRoundCornerSize, Params::MarkerRoundCornerSize);
    }

    return path;
}
