#include "linesdetector.h"

#include <QMap>
#include <QSet>
#include <QDebug>

QList<QPoint> LinesDetector::findHorizontalMarkers(const QImage &source, int spread, int factor)
{
    Q_ASSERT(source.width() > spread && source.height() > spread);
    Q_ASSERT(source.depth() == 32);
    Q_ASSERT(spread > 0);
    Q_ASSERT(factor > 0);

    QList<QPoint> markers;

    // building extremum model
    QMap<int, QMap<int, int>> extremums;

    for (int y = 0; y < source.height(); ++y) {
        const uchar* scan = source.scanLine(y);

        QList<int> gray;
        gray.reserve(source.width());

        for (int x = 0; x < source.width(); x++) {
            const QRgb* pixel = reinterpret_cast<const QRgb*>(scan + x * 4);
            gray << qGray(*pixel);
        }

        QMap<int, int> line_extremums = findExtremums(gray, factor);
        extremums.insert(y, line_extremums);
    }

    // evaluating extremum model
    for (int y = spread; y < source.height() - spread; ++y) {
        const QMap<int, int>& line_extremums = extremums[y];

        foreach (int xx, line_extremums.keys()) {
            bool eligible = true;

            for (int yy = y - spread; yy < y + spread; ++yy) {
                if (!extremums[yy].contains(xx) || extremums[yy][xx] != line_extremums[xx]) {
                    eligible = false;
                    break;
                }
            }

            if (eligible) {
                markers << QPoint(xx, y - spread);
                markers << QPoint(xx, y + spread);
            }
        }
    }

    return markers;
}

QList<QPoint> LinesDetector::findVerticalMarkers(const QImage &source, int spread, int factor)
{
    Q_ASSERT(source.width() > spread && source.height() > spread);
    Q_ASSERT(source.depth() == 32);
    Q_ASSERT(spread > 0);
    Q_ASSERT(factor > 0);

    QList<QPoint> markers;

    // building extremum model
    QMap<int, QMap<int, int>> extremums;

    for (int x = 0; x < source.width(); ++x) {
        QList<int> gray;
        gray.reserve(source.height());

        for (int y = 0; y < source.height(); y++) {
            const uchar* scan = source.scanLine(y);
            const QRgb* pixel = reinterpret_cast<const QRgb*>(scan + x * 4);
            gray << qGray(*pixel);
        }

        QMap<int, int> line_extremums = findExtremums(gray, factor);
        extremums.insert(x, line_extremums);
    }

    // evaluating extremum model
    for (int x = spread; x < source.width() - spread; ++x) {
        const QMap<int, int>& line_extremums = extremums[x];

        foreach (int yy, line_extremums.keys()) {
            bool eligible = true;

            for (int xx = x - spread; xx < x + spread; ++xx) {
                if (!extremums[xx].contains(yy) || extremums[xx][yy] != line_extremums[yy]) {
                    eligible = false;
                    break;
                }
            }

            if (eligible) {
                markers << QPoint(x - spread, yy);
                markers << QPoint(x + spread, yy);
            }
        }
    }

    return markers;
}

QMap<int, int> LinesDetector::findExtremums(QList<int> &values, int factor)
{
    Q_ASSERT(values.length() >= 2);

    // calculate signs vector
    QList<int> signs;
    int v0 = values.first();

    for (int i = 1; i < values.length(); ++i) {
        int v1 = values[i];
        int dv = (v1 - v0) / factor;
        int dv_sign = sign(dv);
        signs << dv_sign;
        v0 = v1;
    }

    // find extremums
    QMap<int, int> map;

    for (int i = 1; i < signs.length(); ++i) {
        if (signs[i - 1] != signs[i]) {
            map.insert(i, signs[i]);
        }
    }

    return map;
}
