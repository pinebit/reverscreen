#pragma once

// std
#include <vector>
#include <opencv/cv.h>

// Qt
#include <QPolygon>
#include <QList>

// Represents an immutable computer vision model
class CvModel
{
public:
    std::vector<std::vector<cv::Point>> const& contours() const {
        return _contours;
    }

    std::vector<cv::Rect> boundingBoxes() const {
        return _boxes;
    }

    // Finds largest object within the given rect.
    QRect findObject(const QRect& rect);

    // Builds convex hull from objects crossed by the given track.
    QPolygon getConvexHull(const QList<QPoint>& track);

private:
    friend class CvModelBuilder;

    CvModel(const std::vector<std::vector<cv::Point>>& _contours);

    std::vector<std::vector<cv::Point>> _contours;
    std::vector<cv::Rect> _boxes;
};
