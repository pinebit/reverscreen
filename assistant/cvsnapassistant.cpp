#include <assistant/cvsnapassistant.h>

#include <CV/cvmodel.h>

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

QPoint CvSnapAssistant::begin(const QPoint &point)
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

QPoint CvSnapAssistant::end(const QRect &rect)
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
