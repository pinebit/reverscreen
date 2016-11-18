#pragma once

// Qt
#include <QObject>
#include <QImage>
#include <QSharedPointer>

// app
#include "cv/cvmodel.h"
#include "cv/cvmodelbuilderoptions.h"

Q_DECLARE_METATYPE(QSharedPointer<CvModel>)

// Implements computer vision logic.
class CvModelBuilder : public QObject
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
