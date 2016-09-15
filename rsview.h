#pragma once

// Qt
#include <QWidget>
#include <QImage>
#include <QSharedPointer>
#include <QPaintEvent>


class SnapAssistant;
class AccentPainter;

// UI control enabling region selection on a given image
class RsView : public QWidget
{
    Q_OBJECT

public:
    RsView(QWidget *parent);

    void setImage(QImage image);
    void setSnapAssistant(QSharedPointer<SnapAssistant> snapAssistant);
    void setAccentPainter(QSharedPointer<AccentPainter> accentPainter);

    QRect selectedRegion() const;

signals:
    void signalMouseMove(const QPoint& point);
    void signalSelectionStarted();
    void signalSelectionFinished();
    void signalSelectionCancelled();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent* event);

private:
    QImage _image;
    QPoint _startPoint;
    QPoint _endPoint;
    QSharedPointer<SnapAssistant> _snapAssistant;
    QSharedPointer<AccentPainter> _accentPainter;
};
