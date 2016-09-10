#pragma once

#include <memory>

#include <QDialog>
#include <QPoint>

class ImageCropWidget : public QDialog
{
    Q_OBJECT

public:
    enum SelectionMode {
        FineSelection,
        SnapSelection,
        AreaSelection
    };

    explicit ImageCropWidget(QWidget* parent);
    void setImage(std::shared_ptr<QPixmap> image, SelectionMode selectionMode);
    QPixmap getCroppedImage();
    bool proceed();

signals:
    void selectionModeChanged(SelectionMode mode);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent* event);

private:
    void drawShaders(QPainter& painter);
    void drawSizeText(QPainter& painter);
    void drawBanner(QPainter& painter);
    QPoint snapStartPoint(const QPoint& p);
    QPoint snapEndPoint(const QPoint& p);
    int distance(const QPoint& p1, const QPoint& p2);
    void finish();

private:
    QPixmap *_source;
    QImage _edges;
    bool _isSelecting;
    QPoint _startPoint;
    QPoint _endPoint;
    SelectionMode _selectionMode;

    QVector<QRect> _boxes;
};
