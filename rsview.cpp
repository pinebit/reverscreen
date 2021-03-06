// Qt
#include <QPainter>
#include <QDebug>
#include <QApplication>

// app
#include "rsview.h"
#include "accent/accentpainter.h"
#include "accent/rectangleaccentpainter.h"
#include "accent/cinemaaccentpainter.h"
#include "selector/selector.h"
#include "selector/cinemaselector.h"
#include "userselection.h"
#include "params.h"

RsView::RsView(QWidget *parent)
    : QWidget(parent)
    , _userSelection(new UserSelection(this))
    , _addMargins(true)
    , _selector(0)
{
    setAutoFillBackground(false);
    setMouseTracking(true);

    parent->installEventFilter(this);

    _cinemaAccentPainter = QSharedPointer<AccentPainter>(new CinemaAccentPainter(Params::ShadeColor));
    _shadeMode = Enabled;

    connect(_userSelection, &UserSelection::signalSelectionChanged, this, &RsView::slotUserSelectionChanged);
}

void RsView::setImage(const QImage& image){
    _image = image;
    _cinemaSelector = QSharedPointer<Selector>(new CinemaSelector(_image.rect()));
    _userSelection->clear();
    _selectionDrawing = QPainterPath();
    _customDrawings = QPainterPath();
    _preferredSelection = QRect();
    _customDrawingsAccentPainter.clear();

    resize(image.size());
    update();
}

void RsView::setSelectionAccentPainter(const QSharedPointer<AccentPainter>& accentPainter) {
    _selectionAccentPainter = accentPainter;
    update();
}

void RsView::setSelectionRenderer(const QSharedPointer<Selector> &selectionRenderer)
{
    _selector = selectionRenderer;
    _userSelection->clear();
    update();
}

void RsView::setShadeMode(RsView::ShadeMode shadeMode)
{
    _shadeMode = shadeMode;
    update();
}

void RsView::setCustomSelectionDrawings(const QList<QPainterPath> &drawings)
{
    _customDrawingsAccentPainter = _selectionAccentPainter;

    QPainterPath united;
    foreach (const QPainterPath& pp, drawings) {
        united = united.united(pp);
    }
    _customDrawings = united;

    update();
}

void RsView::clearSelection()
{
    _userSelection->clear();
    emit signalSelectionCancelled();
    update();
}

QPixmap RsView::renderToPixmap()
{
    QPixmap pixmap = QPixmap::fromImage(_image);
    QPainter painter(&pixmap);

    if (!_customDrawingsAccentPainter.isNull()) {
        _customDrawingsAccentPainter->paint(&painter, _customDrawings);
    }

    return pixmap;
}

void RsView::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    if (_image.isNull()) {
        return;
    }

    QPainter painter(this);

    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.drawImage(0, 0, _image);

    bool hasSelection = _userSelection->isSelected();

    if (_shadeMode != Disabled) {
        if (_shadeMode == Enabled || (_shadeMode == EnabledWhenSelected && hasSelection)) {
            _cinemaAccentPainter->paint(&painter, _cinemaDrawing);
        }
    }

    if (!_customDrawingsAccentPainter.isNull()) {
        _customDrawingsAccentPainter->paint(&painter, _customDrawings);
    }

    if (hasSelection && !_selectionAccentPainter.isNull()) {
        _selectionAccentPainter->paint(&painter, _selectionDrawing);
    }
}

void RsView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _userSelection->clear();
        _userSelection->add(event->pos());
        _preferredSelection = QRect();
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
    default:
        break;
    }

    return false;
}

bool RsView::processingKeyPressEvents(QKeyEvent* keyEvent) {
    switch (keyEvent->key()) {
    case Qt::Key_Escape:
        clearSelection();
        return true;
    case Qt::Key_Space:
        _addMargins = !_addMargins;
    case Qt::Key_Control:
        slotUserSelectionChanged();
        return true;
    }

    return false;
}

bool RsView::processingKeyReleaseEvents(QKeyEvent* keyEvent) {
    switch (keyEvent->key()) {
    case Qt::Key_Control:
        slotUserSelectionChanged();
        return true;
    }

    return false;
}

void RsView::slotUserSelectionChanged()
{
    if (!(QApplication::mouseButtons() & Qt::LeftButton)) {
        // do not handle selection changes when not selecting
        return;
    }

    if (_shadeMode != Disabled) {
        QRect selection = _cinemaSelector->select(_userSelection);
        _cinemaDrawing = _cinemaSelector->render(selection);
    }

    if (!_selector.isNull()) {
        QRect selection = _selector->select(_userSelection);

        Qt::KeyboardModifiers modifiers = QApplication::queryKeyboardModifiers();
        if (Qt::ControlModifier & modifiers) {
            _preferredSelection = _userSelection->getRect();
        }
        else {
            if (_addMargins) {
                const int& m = Params::SelectionMarginSize;
                _preferredSelection = selection.adjusted(-m, -m, m, m);
            }
            else {
                _preferredSelection = selection;
            }
        }

        _selectionDrawing = _selector->render(_preferredSelection);
    }

    emit signalSelectionChanged();

    update();
}
