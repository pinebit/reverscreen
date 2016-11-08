#include "assistant/cvsnapassistant.h"
#include "cv/cvmodel.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <limits>

#include <QDebug>

using namespace cv;
using namespace std;

#define MIN_AREA    32


inline QRect convertRect(const Rect& rect) {
    return QRect(rect.x, rect.y, rect.width, rect.height);
}

inline Point convertPoint(const QPoint& point) {
    return Point(point.x(), point.y());
}

inline QPoint convertPoint(const Point& point) {
    return QPoint(point.x, point.y);
}

inline bool intersection(Point o1, Point p1, Point o2, Point p2)
{
    Point d1 = p1 - o1;
    Point d2 = p2 - o2;

    float cross = d1.x * d2.y - d1.y * d2.x;
    if (abs(cross) < 1e-8) {
        return false;
    }

    return true;
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

QPolygon CvSnapAssistant::marker(const QList<QPoint> &track)
{
    QPolygon polygon;

    if (track.length() < 2) {
        return polygon;
    }

    vector<Point> cloud;

    QPoint prev = track.first();
    for (int i = 1; i < track.length(); i++) {
        QPoint curr = track.at(i);

        for (auto const& box: _model->boundingBoxes()) {
            Rect tr(convertPoint(prev), convertPoint(curr));
            Rect xr = box & tr;
            if (xr.area() > 0) {
                cloud.push_back(Point(box.x, box.y));
                cloud.push_back(Point(box.x + box.width, box.y));
                cloud.push_back(Point(box.x, box.y + box.height));
                cloud.push_back(Point(box.x + box.width, box.y + box.height));
            }
        }
    }

    if (cloud.size() < 4) {
        return polygon;
    }

    vector<Point> hull;
    convexHull(cloud, hull);

    for (auto const& hp: hull) {
        polygon << convertPoint(hp);
    }

    return polygon;
}
