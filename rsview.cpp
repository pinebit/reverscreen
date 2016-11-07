// Qt
#include <QPainter>
#include <QDebug>
#include <QApplication>

// app
#include "rsview.h"
#include "accent/accentpainter.h"
#include "accent/rectangleaccentpainter.h"
#include "accent/selectionaccentpainter.h"
#include "assistant/snapassistant.h"

RsView::RsView(QWidget *parent, bool drawShading)
    : QWidget(parent)
    , _drawShading(drawShading)
    , _keyControlPressed(false)
    , _regionContext(new RegionContext())
    , _highlightSolidLineAccentPainter(new RectangleSolidLineAccentPainter())
    , _selectedDashLineAccentPainter(new SelectionDashLineAccentPainter())
    , _highlightDashLineAccentPainter(new RectangleDashLineAccentPainter())
{
    setAutoFillBackground(false);
    setMouseTracking(true);

    parent->installEventFilter(this);
}

void RsView::setImage(const QImage& image){
    _image = image;

    _snapAssistant.clear();
    _regionContext->clearAll();
    _regionContext->setScopeRegion(_image.rect());

    resize(image.size());
    update();
}

void RsView::setSnapAssistant(const QSharedPointer<SnapAssistant>& snapAssistant){
    _snapAssistant = snapAssistant;
    update();
}

void RsView::setAccentPainter(const QSharedPointer<AccentPainter>& accentPainter) {
    _selectedSolidLineAccentPainter = accentPainter;
    update();
}

QSharedPointer<RegionContext>& RsView::getRegionContext(){
    return _regionContext;
}

QRect RsView::selectedRegion() const {
    return _regionContext->customRegion();
}

void RsView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    if (_image.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.drawImage(0, 0, _image);

    if (!_selectedSolidLineAccentPainter) {
        return;
    }
    const QRect& customRegion = _regionContext->customRegion();
    const QRect& highlightedRegion = _regionContext->highlightedRegion();

    // todo:  add configuration parameter
    if (!_drawShading) {
        if (!RegionContext::isValidRegion(customRegion)){
            return;
        }
    }
    if (customRegion.contains(highlightedRegion, false) || customRegion.intersects(highlightedRegion) ) {
        QRect intersectedRegion = (customRegion.intersects(highlightedRegion))
                ? customRegion.intersected(highlightedRegion)
                : highlightedRegion;
        if (!_keyControlPressed) {
            _selectedDashLineAccentPainter->paint(&painter,  _regionContext->scopeRegion(), customRegion);
            _highlightSolidLineAccentPainter->paint(&painter, customRegion, intersectedRegion);
        } else {
            _selectedSolidLineAccentPainter->paint(&painter, _regionContext->scopeRegion(), customRegion);
            _highlightDashLineAccentPainter->paint(&painter, customRegion, intersectedRegion);
        }
    } else {
        _selectedSolidLineAccentPainter->paint(&painter, _regionContext->scopeRegion(), customRegion);
    }
}

void RsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        const QPoint& point = event->pos();
        _regionContext->setCustomRegion(QRect(point, point));
        update();
        event->accept();
        emit signalSelectionStarted();
    }
}

void RsView::mouseMoveEvent(QMouseEvent *event) {
    event->accept();
    if (event->buttons() == Qt::LeftButton) {
        if (!_snapAssistant.isNull()) {
            const QPoint& point = event->pos();

            QRect customRegion = _regionContext->customRegion(false);
            customRegion.setBottomRight(point);
            _regionContext->setCustomRegion(customRegion);
            _regionContext->setHighlightedRegion(_snapAssistant->snap(_regionContext->customRegion()));

            update();
        }
    } else {
        emit signalMouseMove(event->pos());
    }
}

void RsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        event->accept();
        if (_regionContext->hasSelectedRegion()){
            emit signalSelectionFinished();
        } else {
            emit signalSelectionCancelled();
        }
    }
}

bool RsView::eventFilter(QObject *obj, QEvent *event) {
    Q_UNUSED(obj);
    switch (event->type()) {
    case QEvent::KeyPress:
        return processingKeyPressEvents(static_cast<QKeyEvent*>(event));
    case QEvent::KeyRelease:
        return processingKeyReleaseEvents(static_cast<QKeyEvent*>(event));
    case  QEvent::Wheel:
        return processingWheelEvents(static_cast<QWheelEvent*>(event));
    default:
        break;
    }
    return false;
}

bool RsView::processingKeyPressEvents(QKeyEvent* keyEvent){
    switch (keyEvent->key()){
    case Qt::Key_Escape: {
        _regionContext->clearRegion();
        emit signalSelectionCancelled();
        update();
        return true;
    }
    case Qt::Key_A: {
        if (keyEvent->modifiers() & Qt::ControlModifier) {
            QRect region = _image.rect();
            _regionContext->setCustomRegion(region);
            _regionContext->setHighlightedRegion(region);
            update();
            return true;
        }
        break;
    }
    case Qt::Key_Control: {
        _keyControlPressed = true;
        _regionContext->setRegionType(RegionType::customRegion);
        update();
        return true;
    }
    default:
        break;
    }
    return false;
}

bool RsView::processingKeyReleaseEvents(QKeyEvent* keyEvent){
    switch (keyEvent->key()){
    case Qt::Key_A: {
        if (keyEvent->modifiers() & Qt::ControlModifier) {
            if (_regionContext->hasSelectedRegion()){
                emit signalSelectionFinished();
                return true;
            }
        }
        break;
    }
    case Qt::Key_Control: {
        _keyControlPressed = false;
        _regionContext->setRegionType(RegionType::highlightedRegion);
        update();
        return true;
    }
    default:
        break;
    }
    return false;
}

bool RsView::processingWheelEvents(QWheelEvent* wheelEvent){
    int dx = 0;
    int dy = 0;
    const int step = 1;
    int delta = (wheelEvent->delta() > 0) ? step : -step;
    switch(wheelEvent->buttons()){
      case Qt::LeftButton:
        if (wheelEvent->modifiers() == Qt::NoModifier){
            dx = dy = delta;
            _regionContext->setOffset(dx, dy);
        }
        break;
      case Qt::NoButton:
        if (wheelEvent->modifiers() & Qt::ShiftModifier) {
            dx = delta;
        } else if (wheelEvent->modifiers() & Qt::ControlModifier){
            dy = delta;
        } else {
            dx = dy = delta;
        }
        _regionContext->updateHighlightedRegion(dx, dy);
        break;
    }
    update();
    return true;
}
