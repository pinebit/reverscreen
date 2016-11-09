#pragma once

// Qt
#include <QWidget>
#include <QImage>
#include <QSharedPointer>
#include <QPaintEvent>
#include <QPainterPath>

class SnapAssistant;
class AccentPainter;
class RegionContext;
class SelectionRenderer;
class UserSelection;

// UI control enabling region selection on a given image
class RsView : public QWidget
{
    Q_OBJECT

public:
    enum ShadeMode {
        Enabled,
        EnabledWhenSelected,
        Disabled
    };

    RsView(QWidget *parent);

    void setImage(const QImage& image);
    void setSelectionAccentPainter(const QSharedPointer<AccentPainter>& accentPainter);
    void setSelectionRenderer(const QSharedPointer<SelectionRenderer>& selectionRenderer);
    void setShadeMode(ShadeMode shadeMode);

    const UserSelection* userSelection() const {
        return _userSelection;
    }

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

private slots:
    void slotUserSelectionChanged();

private:
    QImage _image;
    UserSelection* _userSelection;
    QSharedPointer<SelectionRenderer> _selectionRenderer;
    QPainterPath _selectionDrawing;
    QSharedPointer<AccentPainter> _selectionAccentPainter;

    ShadeMode _shadeMode;
    QPainterPath _cinemaDrawing;
    QSharedPointer<AccentPainter> _cinemaAccentPainter;
    QSharedPointer<SelectionRenderer> _cinemaSelectionRenderer;
};
