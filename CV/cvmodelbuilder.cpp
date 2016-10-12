#include <QtConcurrent>

#include "cv/cvmodelbuilder.h"

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

CvModelBuilder::CvModelBuilder(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QSharedPointer<CvModel> >();
}

void CvModelBuilder::buildAsync(const QImage &image, const CvModelBuilderOptions &options)
{
    QtConcurrent::run(this, &CvModelBuilder::build, image, options);
}

void CvModelBuilder::build(const QImage &image, const CvModelBuilderOptions &options)
{
    QImage sourceImage = image;

    if (image.format() != QImage::Format_RGB888) {
        sourceImage = image.convertToFormat(QImage::Format_RGB888);
    }

    Mat cv_image(sourceImage.height(),
                 sourceImage.width(),
                 CV_8UC3,
                 const_cast<uchar*>(sourceImage.bits()),
                 static_cast<size_t>(sourceImage.bytesPerLine()));

    if (options.boostContrast) {
        Mat cv_blur;
        GaussianBlur(cv_image, cv_blur, cv::Size(0, 0), 3);
        addWeighted(cv_image, 1.5, cv_blur, -0.5, 0, cv_blur);
    }

    Mat cv_gray;
    cvtColor(cv_image, cv_gray, COLOR_RGB2GRAY);

    Mat edges;
    Canny(cv_gray, edges, options.cannyThreshold1, options.cannyThreshold2);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    CvModel* model = new CvModel(contours);
    emit signalBuildCompleted(QSharedPointer<CvModel>(model));
}
