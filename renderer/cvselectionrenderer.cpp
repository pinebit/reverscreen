#include "cvselectionrenderer.h"
#include "userselection.h"

CvSelectionRenderer::CvSelectionRenderer(QSharedPointer<CvModel> cvModel)
    : _cvModel(cvModel)
{
}

QPainterPath CvSelectionRenderer::render(const UserSelection *selection)
{
    QPainterPath path;

    if (selection->isSelected()) {
        QRect rect = selection->getRect();
        if (!rect.isNull()) {
            QRect cvRect = _cvModel->findObject(rect);
            path.addRect(cvRect);
        }
    }

    return path;
}
