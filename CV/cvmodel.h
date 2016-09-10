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

    std::vector<cv::Rect> boundingBoxes() const;

private:
    friend class CvModelBuilder;

    CvModel();

    std::vector<std::vector<cv::Point>> _contours;
    std::vector<cv::Vec4i> _hierarchy;
};
