// Qt
#include <QPainter>
#include <QDebug>

// app
#include <rsview.h>
#include <accent/accentpainter.h>
#include <assistant/snapassistant.h>


static const QColor RegionColor = Qt::red;
static const QColor ShaderColor = QColor::fromRgba(0x50a0a0a0);

static const int CheckerSize = 8;

RsView::RsView(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(false);
    setMouseTracking(true);
}

void RsView::setImage(QImage image)
{
    _snapAssistant.clear();
    _image = image;
    _startPoint = _endPoint = QPoint(-1, -1);

    resize(image.size());

    update();
}

void RsView::setSnapAssistant(QSharedPointer<SnapAssistant> snapAssistant)
{
    _snapAssistant = snapAssistant;
    update();
}

void RsView::setAccentPainter(QSharedPointer<AccentPainter> accentPainter)
{
    _accentPainter = accentPainter;
    update();
}

QRect RsView::selectedRegion() const
{
    return QRect(_startPoint, _endPoint).normalized();
}

void RsView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (_image.isNull()) {
        return;
    }

    QPainter painter(this);

    painter.drawImage(0, 0, _image);

    if (_startPoint != _endPoint && _accentPainter != NULL) {
        QRect scope(0, 0, _image.width(), _image.height());
        QRect region(_startPoint, _endPoint);
        _accentPainter->paint(&painter, scope, region.normalized());
    }
}

void RsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!_snapAssistant.isNull()) {
            _endPoint = _startPoint = _snapAssistant->begin(event->pos());
            update();
            event->accept();

            emit signalSelectionStarted();
        }
    }
}

void RsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        event->accept();
        emit signalSelectionFinished();
    }
}

void RsView::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();

    if (event->buttons() == Qt::LeftButton) {
        if (!_snapAssistant.isNull()) {
            _endPoint = _snapAssistant->end(QRect(_startPoint, event->pos()));
            update();
        }
    }
    else {
        emit signalMouseMove(event->pos());
    }
}

void RsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        event->accept();
        emit signalSelectionCancelled();
    }
}

void RsView::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
