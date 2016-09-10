#ifndef LINESDETECTOR_H
#define LINESDETECTOR_H

#include <QPixmap>
#include <QList>
#include <QLine>
#include <QPoint>
#include <QHash>

class LinesDetector
{
public:
    QList<QPoint> findHorizontalMarkers(const QImage& source, int spread, int factor);
    QList<QPoint> findVerticalMarkers(const QImage& source, int spread, int factor);

private:
    QMap<int, int> findExtremums(QList<int>& values, int factor);

    template <typename T> int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }
};

#endif // LINESDETECTOR_H
