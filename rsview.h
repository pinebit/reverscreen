#pragma once

// Qt
#include <QWidget>
#include <QImage>
#include <QSharedPointer>
#include <QPaintEvent>

#include "context/regioncontext.h"

class SnapAssistant;
class AccentPainter;
class RegionContext;

// UI control enabling region selection on a given image
class RsView : public QWidget
{
    Q_OBJECT

public:
    RsView(QWidget *parent, bool drawShading);

    void setImage(const QImage& image);
    void setSnapAssistant(const QSharedPointer<SnapAssistant>& snapAssistant);
    void setAccentPainter(const QSharedPointer<AccentPainter>& accentPainter);
    QSharedPointer<RegionContext>& getRegionContext();
    
    QRect selectedRegion() const;

signals:
    void signalMouseMove(const QPoint& point);
    void signalSelectionStarted();
    void signalSelectionFinished();
    void signalSelectionCancelled();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool processingKeyPressEvents(QKeyEvent* keyEvent);
    bool processingKeyReleaseEvents(QKeyEvent* keyEvent);
    bool processingWheelEvents(QWheelEvent* wheelEvent);

private:
    QImage _image;

    bool _drawShading;
    bool _keyControlPressed;

    QSharedPointer<RegionContext> _regionContext;
    QSharedPointer<SnapAssistant> _snapAssistant;

    QSharedPointer<AccentPainter> _selectedSolidLineAccentPainter;
    QSharedPointer<AccentPainter> _highlightSolidLineAccentPainter;
    QSharedPointer<AccentPainter> _selectedDashLineAccentPainter;
    QSharedPointer<AccentPainter> _highlightDashLineAccentPainter;
};
