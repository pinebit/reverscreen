#include <CV/cvmodel.h>

#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


vector<Rect> CvModel::boundingBoxes() const
{
    vector<Rect> boxes;
    boxes.reserve(_contours.size());

    for (auto const& contour: _contours) {
        Rect rt = boundingRect(contour);
        boxes.push_back(rt);
    }

    return boxes;
}

CvModel::CvModel()
{
}
