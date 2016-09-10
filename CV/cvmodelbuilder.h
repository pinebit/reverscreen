#pragma once

// Qt
#include <QObject>
#include <QImage>
#include <QSharedPointer>

// app
#include <CV/cvmodel.h>
#include <CV/cvmodelbuilderoptions.h>


// Implements computer vision logic.
class CvModelBuilder : QObject
{
    Q_OBJECT

public:
    CvModelBuilder(QObject* parent);

    void buildAsync(const QImage& image, const CvModelBuilderOptions& options);

signals:
    void signalBuildCompleted(QSharedPointer<CvModel> model);

private:
    void build(const QImage& image, const CvModelBuilderOptions& options);
};
