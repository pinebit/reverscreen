// Qt
#include <QPainter>
#include <QDebug>
#include <QApplication>

// app
#include "rsview.h"
#include "accent/accentpainter.h"
#include "accent/rectangleaccentpainter.h"
#include "accent/cinemaaccentpainter.h"
#include "renderer/selectionrenderer.h"
#include "renderer/cinemaselectionrenderer.h"
#include "userselection.h"

RsView::RsView(QWidget *parent)
    : QWidget(parent)
    , _userSelection(new UserSelection(this))
    , _selectionRenderer(0)
{
    setAutoFillBackground(false);
    setMouseTracking(true);

    parent->installEventFilter(this);

    _cinemaAccentPainter = QSharedPointer<AccentPainter>(new CinemaAccentPainter(Qt::gray));
    _cinemaEnabled = true;

    connect(_userSelection, &UserSelection::signalSelectionChanged, this, &RsView::slotUserSelectionChanged);
}

void RsView::setImage(const QImage& image){
    _image = image;
    _cinemaSelectionRenderer = QSharedPointer<SelectionRenderer>(new CinemaSelectionRenderer(_image.rect()));
    _userSelection->clear();
    _selectionDrawing = QPainterPath();

    resize(image.size());
    update();
}

void RsView::setSelectionAccentPainter(const QSharedPointer<AccentPainter>& accentPainter) {
    _selectionAccentPainter = accentPainter;
    update();
}

void RsView::setSelectionRenderer(const QSharedPointer<SelectionRenderer> &selectionRenderer)
{
    _selectionRenderer = selectionRenderer;
    _userSelection->clear();
    update();
}

void RsView::setSelectionShading(bool enabled)
{
    _cinemaEnabled = enabled;
    update();
}

void RsView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    if (_image.isNull()) {
        return;
    }

    QPainter painter(this);

    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.drawImage(0, 0, _image);

    if (_cinemaEnabled) {
        _cinemaAccentPainter->paint(&painter, _cinemaDrawing);
    }

    if (_userSelection->isSelected() && !_selectionAccentPainter.isNull()) {
        _selectionAccentPainter->paint(&painter, _selectionDrawing);
    }
}

void RsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _userSelection->clear();
        _userSelection->add(event->pos());
        event->accept();
    }
}

void RsView::mouseMoveEvent(QMouseEvent *event) {
    event->accept();

    if (event->buttons() == Qt::LeftButton) {
        _userSelection->add(event->pos());
    } else {
        emit signalMouseMove(event->pos());
    }
}

void RsView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        event->accept();
        emit signalSelectionFinished();
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
        _userSelection->clear();
        update();
        return true;
    }
    /*
    case Qt::Key_Control: {
        if (_mouseButtonPressed & Qt::LeftButton) {
            _regionContext->setRegionType(RegionType::customRegion);
        }
        update();
        return true;
    }
    */
    default:
        break;
    }

    return false;
}

bool RsView::processingKeyReleaseEvents(QKeyEvent* keyEvent) {
    /*
    switch (keyEvent->key()){
    case Qt::Key_Control: {
        if (_mouseButtonPressed & Qt::LeftButton) {
            _regionContext->setRegionType(RegionType::highlightedRegion);
        }
        update();
        return true;
    }
    default:
        break;
    }
    */

    return false;
}

bool RsView::processingWheelEvents(QWheelEvent* wheelEvent) {
    /*
    int dx = 0;
    int dy = 0;
    const int step = 1;
    int delta = (wheelEvent->delta() > 0) ? step : -step;
    switch(wheelEvent->buttons()){
      case Qt::LeftButton:
        if (wheelEvent->modifiers() == Qt::NoModifier){
            dx = dy = delta;
            _regionContext->setOffset(dx, dy);
            update();
            return true;
        }
      default:
        break;
    }
    */

    return false;
}

void RsView::slotUserSelectionChanged()
{
    if (_cinemaEnabled) {
        _cinemaDrawing = _cinemaSelectionRenderer->render(_userSelection);
    }

    if (!_selectionRenderer.isNull()) {
        _selectionDrawing = _selectionRenderer->render(_userSelection);
    }

    update();
}
