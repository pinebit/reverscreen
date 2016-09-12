#include <snapselectionstrategy.h>

#include <CV/cvmodel.h>

#include <opencv2/imgproc/imgproc.hpp>

#include <limits>

#include <QRect>

using namespace cv;
using namespace std;

#define MIN_AREA    32


inline QRect convertRect(const Rect& rect) {
    return QRect(rect.x, rect.y, rect.width, rect.height);
}

inline Point convertPoint(const QPoint& point) {
    return Point(point.x(), point.y());
}


SnapSelectionStrategy::SnapSelectionStrategy(QSharedPointer<CvModel> model)
    : _model(model)
{
}

QRect SnapSelectionStrategy::hint(const QPoint &point)
{
    Point cv_point = convertPoint(point);

    int min_area = numeric_limits<int>::max();
    Rect min_rect;

    for (auto const& box: _model->boundingBoxes()) {
        if (box.contains(cv_point)) {
            int area = box.area();
            if (area < MIN_AREA) {
                continue;
            }

            if (area < min_area) {
                min_area = area;
                min_rect = box;
            }
        }
    }

    return convertRect(min_rect);
}

QPoint SnapSelectionStrategy::begin(const QPoint &point)
{
    int min_dx = numeric_limits<int>::max();
    int min_dy = numeric_limits<int>::max();
    int sx = point.x();
    int sy = point.y();

    for (auto const& box: _model->boundingBoxes()) {
        if (box.area() < MIN_AREA) {
            continue;
        }

        QList<QPoint> points;
        QRect qrect = convertRect(box);
        points << qrect.topLeft() << qrect.topRight() << qrect.bottomLeft() << qrect.bottomRight();

        foreach (const QPoint& cp, points) {
            int ml = (point - cp).manhattanLength();

            if (ml < min_dx) {
                min_dx = ml;
                sx = cp.x();
            }

            if (ml < min_dy) {
                min_dy = ml;
                sy = cp.y();
            }
        }
    }

    return QPoint(sx, sy);
}

QPoint SnapSelectionStrategy::end(const QRect &rect)
{
    int min_dx = numeric_limits<int>::max();
    int min_dy = numeric_limits<int>::max();
    int sx = rect.right();
    int sy = rect.bottom();

    for (auto const& box: _model->boundingBoxes()) {
        if (box.area() < MIN_AREA) {
            continue;
        }

        QList<QPoint> points;
        QRect qbox = convertRect(box);
        points << qbox.topLeft() << qbox.topRight() << qbox.bottomLeft() << qbox.bottomRight();

        foreach (const QPoint& cp, points) {
            if (!rect.contains(cp)) {
                continue;
            }

            int dx = qAbs(cp.x() - rect.right());
            int dy = qAbs(cp.y() - rect.bottom());

            if (dx < min_dx) {
                min_dx = dx;
                sx = cp.x();
            }

            if (dy < min_dy) {
                min_dy = dy;
                sy = cp.y();
            }
        }
    }

    QPoint result(sx, sy);
    if (result == rect.topLeft()) {
        result = rect.bottomRight();
    }

    return result;
}

QRect SnapSelectionStrategy::adjust(const QRect &rect)
{
    return rect;
}
