#include "cvselector.h"
#include "userselection.h"

CvSelector::CvSelector(QSharedPointer<CvModel> cvModel)
    : _cvModel(cvModel)
{
}

QRect CvSelector::select(const UserSelection *selection)
{
    if (selection->isSelected()) {
        QRect rect = selection->getRect();
        if (!rect.isNull()) {
            QRect cvRect = _cvModel->findObject(rect);
            if (cvRect.isNull()) {
                return rect;
            }
            else {
                return cvRect;
            }
        }
    }

    return QRect();
}

QPainterPath CvSelector::render(const QRect &selection)
{
    QPainterPath path;

    if (!selection.isNull()) {
        path.addRect(selection.adjusted(0, 0, -1, -1));
    }

    return path;
}
