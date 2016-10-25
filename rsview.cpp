// Qt
#include <QPainter>
#include <QDebug>
#include <QApplication>

// app
#include "rsview.h"
#include "accent/accentpainter.h"
#include "assistant/snapassistant.h"

RsView::RsView(QWidget *parent, bool modeScreenshot)
    : QWidget(parent)
    , _modeScreenshot(modeScreenshot)
    , _keyControlPress(false)
    , _keyShiftPress(false)
    , _regionContext(new RegionContext()){
    setAutoFillBackground(false);
    setMouseTracking(true);

    parent->installEventFilter(this);
}

void RsView::setImage(const QImage& image){
    _image = image;

    _regionContext->clear();
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

bool RsView::modeScreenshot() const{
    return _modeScreenshot;
}

void RsView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    if (_image.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.drawImage(0, 0, _image);

    if (_accentPainter != NULL) {
        if (_modeScreenshot && !_keyControlPress){
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
        emit signalSelectionFinished();
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
            event->accept();
            emit signalSelectionCancelled();
            return true;
        }
        case Qt::Key_Up:{
            if (_keyControlPress) {
                _regionContext->updateHighlightedRegion(0, -1);
            } else if (_keyShiftPress) {
                _regionContext->changeHighlightedRegion(0, -1);
            } else{
                _regionContext->translateHighlightedRegion(0, -1);
            }
            update();
            return true;
        }
        case Qt::Key_Left:{
            if (_keyControlPress) {
                _regionContext->updateHighlightedRegion(-1, 0);
            } else if (_keyShiftPress) {
                _regionContext->changeHighlightedRegion(-1, 0);
            } else{
                _regionContext->translateHighlightedRegion(-1, 0);
            }
            update();
            return true;
        }
        case Qt::Key_Right:{
            if (_keyControlPress) {
                _regionContext->updateHighlightedRegion(1, 0);
            } else if (_keyShiftPress) {
                _regionContext->changeHighlightedRegion(1, 0);
            } else{
                _regionContext->translateHighlightedRegion(1, 0);
            }
            update();
            return true;
        }
        case Qt::Key_Down:{
            if (_keyControlPress) {
                _regionContext->updateHighlightedRegion(0, 1);
            } else if (_keyShiftPress) {
                _regionContext->changeHighlightedRegion(0, 1);
            } else{
                _regionContext->translateHighlightedRegion(0, 1);
            }
            update();
            return true;
        }
        case Qt::Key_Shift: {
            _keyShiftPress = true;
            update();
            return true;
        }
        case Qt::Key_Control: {
            _keyControlPress = true;
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
            _keyShiftPress = false;
            update();
            return true;
        }
        case Qt::Key_Control: {
            _keyControlPress = false;
            update();
            return true;
        }
        default:
            break;
        }
    }
    return false;
}

void RsView::wheelEvent(QWheelEvent *event) {
    int delta = 1;
    if (event->modifiers() & Qt::ShiftModifier) {
        delta = 3;
    } else  if (event->modifiers() & Qt::ControlModifier){
        delta = 2;
    }
    int ds =(event->delta() > 0) ? delta : -delta;
    _regionContext->updateHighlightedRegion(ds, ds);
    update();
}
