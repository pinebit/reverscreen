#pragma once

// Qt
#include <QWidget>
#include <QImage>
#include <QSharedPointer>
#include <QPaintEvent>
#include <QCursor>

// app
#include <selectionstrategy.h>


// UI control enabling region selection on a given image
class RegionSelector : public QWidget
{
    Q_OBJECT

public:
    explicit RegionSelector(QWidget *parent, const QImage& image);

    void setSelectionStrategy(QSharedPointer<SelectionStrategy> strategy, QCursor cursor);
    QImage getSelectedImage();

signals:
    void signalRegionUpdated(QRect region);
    void signalSelectionFinished();
    void signalSelectionCancelled();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent* event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    void drawRegionRect(QPainter& painter);
    void drawRegionShaders(QPainter& painter);
    void drawHintRect(QPainter& painter);

private:
    QImage _image;
    QSharedPointer<SelectionStrategy> _strategy;
    QPoint _startPoint;
    QPoint _endPoint;
};