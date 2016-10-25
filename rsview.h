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
    RsView(QWidget *parent, bool modeScreenshot);

    void setImage(const QImage& image);
    void setSnapAssistant(const QSharedPointer<SnapAssistant>& snapAssistant);
    void setAccentPainter(const QSharedPointer<AccentPainter>& accentPainter);
    void setRegionContext(const QSharedPointer<RegionContext>& regionContext);
    QSharedPointer<RegionContext>& getRegionContext();
    
    QRect selectedRegion() const;
    QRect highlightedRegion() const;

    bool modeScreenshot() const;

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
    void wheelEvent(QWheelEvent* event);

private:
    QImage _image;

    bool _modeScreenshot;
    bool _keyControlPress;
    bool _keyShiftPress;
    QSharedPointer<AccentPainter> _accentPainter;
    QSharedPointer<RegionContext> _regionContext;
};
