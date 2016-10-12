#include "cv/cvmodel.h"

using namespace std;
using namespace cv;


CvModel::CvModel(const std::vector<std::vector<cv::Point>>& _contours)
    : _contours(_contours)
{
    _boxes.reserve(_contours.size());

    for (auto const& contour: _contours) {
        Rect rt = boundingRect(contour);
        _boxes.push_back(rt);
    }
}
