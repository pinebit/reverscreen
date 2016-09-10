#pragma once

// Qt
#include <QWidget>
#include <QImage>
#include <QSharedPointer>
#include <QPaintEvent>

class CvModel;


// UI control enabling region selection on a given image
class RegionSelector : public QWidget
{
    Q_OBJECT

public:
    explicit RegionSelector(QWidget *parent, const QImage& image, QSharedPointer<CvModel> model);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage _image;
    QSharedPointer<CvModel> _model;
};
