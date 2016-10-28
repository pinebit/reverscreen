// Qt
#include <QPainter>
#include <QDebug>
#include <QApplication>

// app
#include "rsview.h"
#include "accent/accentpainter.h"
#include "assistant/snapassistant.h"

RsView::RsView(QWidget *parent, bool fullWidgetMode)
    : QWidget(parent)
    , _keyControlPressed(false)
    , _keyShiftPressed(false)
    , _regionContext(new RegionContext(fullWidgetMode)){
    setAutoFillBackground(false);
    setMouseTracking(true);

    parent->installEventFilter(this);
}

void RsView::setImage(const QImage& image){
    _image = image;

    _regionContext->clearAll();
    _regionContext->setScopeRegion(_image.rect());

    resize(image.size());
    update();
}

void RsView::setSnapAssistant(const QSharedPointer<SnapAssistant>& snapAssistant){
    _regionContext->setSnapAssistant(snapAssistant);
    update();
}

void RsView::setAccentPainter(const QSharedPointer<AccentPainter>& accentPainter) {
    _accentPainter = accentPainter;
    update();
}

void RsView::setRegionContext(const QSharedPointer<RegionContext>& regionContext){
    _regionContext = regionContext;
}

QSharedPointer<RegionContext>& RsView::getRegionContext(){
    return _regionContext;
}

QRect RsView::selectedRegion() const {
    return _regionContext->selectedRegion();
}

QRect RsView::highlightedRegion() const {
    return _regionContext->highlightedRegion();
}

void RsView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    if (_image.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.drawImage(0, 0, _image);

    if (_accentPainter != NULL) {
        if (!_keyControlPressed){ // _regionContext->fullWidgetMode() &&
            _accentPainter->paint(&painter, _regionContext.data());
        } else {
            _accentPainter->paint(&painter, _regionContext->scopeRegion(), _regionContext->highlightedRegion());
        }
    }
}

void RsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!_regionContext->isNull()) {
            _regionContext->updateStartPoint(event->pos());
            update();
            event->accept();
            emit signalSelectionStarted();
        }
    }
}

void RsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        event->accept();
        if (!_regionContext->hasSelectedRegion()){
            emit signalSelectionCancelled();
        } else {
            emit signalSelectionFinished();
        }
    }
}

void RsView::mouseMoveEvent(QMouseEvent *event) {
    event->accept();

    if (event->buttons() == Qt::LeftButton) {
        if (!_regionContext->isNull()) {
            _regionContext->updateEndPoint(event->pos());
            update();
        }
    } else {
        emit signalMouseMove(event->pos());
    }
}

bool RsView::eventFilter(QObject *obj, QEvent *event) {
    Q_UNUSED(obj);
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key()){
        case Qt::Key_Escape: {
            _regionContext->clearRegion();
            emit signalSelectionCancelled();
            update();
            return true;
        }
        case Qt::Key_A: {
            if (keyEvent->modifiers() & Qt::ControlModifier) {
                QRect region = _image.rect().adjusted(1,1,-1,-1);
                _regionContext->setSelectedRegion(region);
                _regionContext->setHighlightedRegion(region);
                update();
            }
            return true;
        }
        case Qt::Key_Up:{
            if (keyEvent->modifiers() & Qt::ControlModifier) {
                _regionContext->updateHighlightedRegion(0, -1);
            } else if (keyEvent->modifiers() & Qt::ShiftModifier) {
                _regionContext->changeHighlightedRegion(0, -1);
            } else{
                _regionContext->translateHighlightedRegion(0, -1);
            }
            update();
            return true;
        }
        case Qt::Key_Left:{
            if (keyEvent->modifiers() & Qt::ControlModifier) {
                _regionContext->updateHighlightedRegion(-1, 0);
            } else if (keyEvent->modifiers() & Qt::ShiftModifier) {
                _regionContext->changeHighlightedRegion(-1, 0);
            } else{
                _regionContext->translateHighlightedRegion(-1, 0);
            }
            update();
            return true;
        }
        case Qt::Key_Right:{
            if (keyEvent->modifiers() & Qt::ControlModifier) {
                _regionContext->updateHighlightedRegion(1, 0);
            } else if (keyEvent->modifiers() & Qt::ShiftModifier) {
                _regionContext->changeHighlightedRegion(1, 0);
            } else{
                _regionContext->translateHighlightedRegion(1, 0);
            }
            update();
            return true;
        }
        case Qt::Key_Down:{
            if (keyEvent->modifiers() & Qt::ControlModifier) {
                _regionContext->updateHighlightedRegion(0, 1);
            } else if (keyEvent->modifiers() & Qt::ShiftModifier) {
                _regionContext->changeHighlightedRegion(0, 1);
            } else{
                _regionContext->translateHighlightedRegion(0, 1);
            }
            update();
            return true;
        }
        case Qt::Key_Shift: {
            _keyShiftPressed = true;
            update();
            return true;
        }
        case Qt::Key_Control: {
            _keyControlPressed = true;
            update();
            return true;
        }
        default:
            break;
        }
    }
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key()){
        case Qt::Key_Shift: {
            _keyShiftPressed = false;
            update();
            return true;
        }
        case Qt::Key_Control: {
            _keyControlPressed = false;
            update();
            return true;
        }
        default:
            break;
        }
    }
    if (event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
        int dx = 0;
        int dy = 0;
        const int step = 1;
        int delta = (wheelEvent->delta() > 0) ? step : -step;
        if (wheelEvent->modifiers() & Qt::ShiftModifier) {
            dx = delta;
        } else  if (wheelEvent->modifiers() & Qt::ControlModifier){
            dy = delta;
        } else {
            dx = dy = delta;
        }
        _regionContext->updateHighlightedRegion(dx, dy);
        update();
        return true;
    }
    return false;
}

bool RsView::usedHighlightedRegion() const {
    return _keyControlPressed;
}
