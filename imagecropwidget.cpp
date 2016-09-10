#include "imagecropwidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QRect>
#include <QDebug>
#include <QGuiApplication>
#include <QDebug>

#include <vector>
#include <limits>

#include <asmopencv.h>
#include <linesdetector.h>

#define MAX_DISTANCE (256 * 256)
#define MIN_AREA     (4*4)

using namespace cv;
using namespace std;

ImageCropWidget::ImageCropWidget(QWidget* parent):
    QDialog(parent)
{
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool | Qt::SubWindow);
    this->setCursor(Qt::CrossCursor);
    this->setWindowState(Qt::WindowFullScreen);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
}

void ImageCropWidget::setImage(std::shared_ptr<QPixmap> image)
{
    _source = new QPixmap(*image);
    this->setGeometry(0, 0, _source->width(), _source->height());
    this->repaint();

    Mat cv_rgb = QPixmapToCvMat(*_source);
    Mat cv_gb;
    GaussianBlur(cv_rgb, cv_gb, cv::Size(0, 0), 3);
    addWeighted(cv_rgb, 1.5, cv_gb, -0.5, 0, cv_gb);

    Mat cv_gray;
    cvtColor(cv_rgb, cv_gray, COLOR_RGB2GRAY);

    Mat edges;
    Canny(cv_gray, edges, 100, 300);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    // _boxes << QRect(0, 0, _source->width(), _source->height());

    vector<vector<Point>>::iterator it = contours.begin();
    while (it != contours.end()) {
        const vector<Point>& c = *it;

        Rect rt = boundingRect(c);
        if (rt.width * rt.height < MIN_AREA) {
            ++it;
            continue;
        }

        QRect qrt(rt.x, rt.y, rt.width, rt.height);
        _boxes << qrt;

        ++it;
    }
}

bool ImageCropWidget::proceed()
{
    return this->exec();
}

QPixmap ImageCropWidget::getCroppedImage()
{
    return *_source;
    delete _source;
}

void ImageCropWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED( event );

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    painter.drawPixmap(0,0,_source->width(),_source->height(),*_source);

    if (_isSelecting)
    {
        QPen pen(Qt::red);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRect(_startPoint.x(), _startPoint.y(),
                         _endPoint.x()-_startPoint.x(), _endPoint.y()-_startPoint.y());

        drawShaders(painter);
    }

    if (QGuiApplication::keyboardModifiers() & Qt::AltModifier) {
        QPen pen(Qt::blue);
        pen.setWidth(1);
        painter.setPen(pen);
        painter.drawRects(_boxes.data(), _boxes.length());
    }

    drawBanner(painter);
}

void ImageCropWidget::mousePressEvent(QMouseEvent *event)
{
    event->accept();

    if (event->button() == Qt::LeftButton)
    {
        _isSelecting = true;
        _startPoint = snapStartPoint(event->pos());
        _endPoint = _startPoint + QPoint(4, 4);

        this->update();
    }

    if (event->button() == Qt::RightButton) {
        finish();
    }
}

void ImageCropWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (_isSelecting) {
        _endPoint = snapEndPoint(event->pos());

        this->update();

        event->accept();
    }
}

void ImageCropWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
}

void ImageCropWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        finish();
    }
    else if (event->key() == Qt::Key_Escape) {
        this->reject();
    }
    else {
        QDialog::keyPressEvent(event);
    }
}

void ImageCropWidget::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 16;
    int len = numDegrees.x() + numDegrees.y();
    QPoint dp(len, len);
    _startPoint -= dp;
    _endPoint += dp;

    event->accept();

    this->update();
}

void ImageCropWidget::drawShaders(QPainter& painter)
{
    QBrush brush(QColor::fromRgba(0x50a0a0a0));

    int min_x = qMin(_startPoint.x(), _endPoint.x());
    int min_y = qMin(_startPoint.y(), _endPoint.y());
    int max_x = qMax(_startPoint.x(), _endPoint.x());
    int max_y = qMax(_startPoint.y(), _endPoint.y());

    // top shader
    painter.fillRect(0, 0, _source->width(), min_y, brush);

    // bottom shader
    painter.fillRect(0, max_y + 1, _source->width(), _source->height() - max_y, brush);

    // left shader
    painter.fillRect(0, min_y, min_x, max_y - min_y + 1, brush);

    // right shader
    painter.fillRect(max_x + 1, min_y, _source->width() - max_x, max_y - min_y + 1, brush);
}

void ImageCropWidget::drawSizeText(QPainter &painter)
{
    QRect rt = QRect(_startPoint, _endPoint);
    int h = qAbs(_startPoint.y() - _endPoint.y()) + 1;
    int w = qAbs(_startPoint.x() - _endPoint.x()) + 1;
    painter.drawText(rt, Qt::AlignCenter, QString("%1x%2").arg(w).arg(h));
}

void ImageCropWidget::drawBanner(QPainter &painter)
{
    QBrush brush(QColor::fromRgba(0x80803030));

    // banner background
    int w = _source->width() / 2;
    int x = w / 2;
    QRect rect = QRect(x, 0, w, 40);

    painter.fillRect(rect, brush);

    QString text = "Use mouse to select and adjust a region. Press Enter when done.";

    QPen pen(Qt::white);
    painter.setPen(pen);
    QFont font("Calibri", 14);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, text);
}

QPoint ImageCropWidget::snapStartPoint(const QPoint& p)
{
    int min_dx = numeric_limits<int>::max();
    int min_dy = numeric_limits<int>::max();
    int sx = p.x();
    int sy = p.y();

    foreach (const QRect& rt, _boxes) {
        QList<QPoint> points;
        points << rt.topLeft() << rt.topRight() << rt.bottomLeft() << rt.bottomRight();

        foreach (const QPoint& cp, points) {
            int ml = (p - cp).manhattanLength();

            if (ml < min_dx) {
                min_dx = ml;
                sx = cp.x();
            }

            if (ml < min_dy) {
                min_dy = ml;
                sy = cp.y();
            }
        }
    }

    return QPoint(sx, sy);
}

QPoint ImageCropWidget::snapEndPoint(const QPoint& p)
{
    QRect frame(_startPoint, p);
    int min_dx = numeric_limits<int>::max();
    int min_dy = numeric_limits<int>::max();
    int sx = p.x();
    int sy = p.y();

    foreach (const QRect& rt, _boxes) {
        QList<QPoint> points;
        points << rt.topLeft() << rt.topRight() << rt.bottomLeft() << rt.bottomRight();

        foreach (const QPoint& cp, points) {
            if (!frame.contains(cp)) {
                continue;
            }

            int dx = qAbs(cp.x() - p.x());
            int dy = qAbs(cp.y() - p.y());

            if (dx < min_dx) {
                min_dx = dx;
                sx = cp.x();
            }

            if (dy < min_dy) {
                min_dy = dy;
                sy = cp.y();
            }
        }
    }

    QPoint result(sx, sy);
    if (result == _startPoint) {
        result = p;
    }

    return result;
}

int ImageCropWidget::distance(const QPoint& p1, const QPoint& p2)
{
    int dx = p2.x() - p1.x();
    int dy = p2.y() - p1.y();
    return dx * dx + dy * dy;
}

void ImageCropWidget::finish()
{
    _isSelecting = false;

    qint32 x, y, w, h, x1, y1, x2, y2;
    x1=_startPoint.x();
    x2=_endPoint.x();
    y1=_startPoint.y();
    y2=_endPoint.y();

    x = x1<x2 ? x1 : x2;
    y = y1<y2 ? y1 : y2;
    w = x1<x2 ? x2-x1 : x1-x2;
    h = y1<y2 ? y2-y1 : y1-y2;

    *_source = _source->copy(x, y, w, h);

    this->accept();
}
