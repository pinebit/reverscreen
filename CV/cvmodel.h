#pragma once

// std
#include <vector>

// opencv
#include <opencv2/core.hpp>


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

private:
    friend class CvModelBuilder;

    CvModel(const std::vector<std::vector<cv::Point>>& _contours);

    std::vector<std::vector<cv::Point>> _contours;
    std::vector<cv::Rect> _boxes;
};
