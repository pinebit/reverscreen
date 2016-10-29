#include "assistant/cvsnapassistant.h"
#include "cv/cvmodel.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <limits>

using namespace cv;
using namespace std;

#define MIN_AREA    32


inline QRect convertRect(const Rect& rect) {
    return QRect(rect.x, rect.y, rect.width, rect.height);
}

inline Point convertPoint(const QPoint& point) {
    return Point(point.x(), point.y());
}

CvSnapAssistant::CvSnapAssistant(QSharedPointer<CvModel> model)
    : _model(model)
{
}

QRect CvSnapAssistant::snap(const QRect &rect)
{
    QRect unitedRect;
    bool unitedRectSet = false;

    for (auto const& box: _model->boundingBoxes()) {
        if (box.area() < MIN_AREA) {
            continue;
        }

        QRect cvRect = convertRect(box);
        if (!rect.contains(cvRect)) {
            continue;
        }

        if (!unitedRectSet) {
            unitedRectSet = true;
            unitedRect = cvRect;
        }
        else {
            unitedRect = unitedRect.united(cvRect);
        }
    }

    if (unitedRectSet) {
        return unitedRect;
    }

    return rect;
}
