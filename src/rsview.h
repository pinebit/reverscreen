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
class Selector;
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
    void setSelectionRenderer(const QSharedPointer<Selector>& selectionRenderer);
    void setShadeMode(ShadeMode shadeMode);
    void setCustomSelectionDrawings(const QList<QPainterPath>& drawings);

    const UserSelection* userSelection() const {
        return _userSelection;
    }

    QRect preferredSelection() const {
        return _preferredSelection;
    }

    QPainterPath selectionDrawing() const {
        return _selectionDrawing;
    }

    void clearSelection();

    QPixmap renderToPixmap();

signals:
    void signalMouseMove(const QPoint& point);
    void signalSelectionChanged();
    void signalSelectionCancelled();
    void signalSelectionFinished();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool processingKeyPressEvents(QKeyEvent* keyEvent);
    bool processingKeyReleaseEvents(QKeyEvent* keyEvent);

private slots:
    void slotUserSelectionChanged();

private:
    QImage _image;
    UserSelection* _userSelection;
    ShadeMode _shadeMode;
    QRect _preferredSelection;
    bool _addMargins;

    QPainterPath _customDrawings;
    QPainterPath _cinemaDrawing;
    QPainterPath _selectionDrawing;

    QSharedPointer<Selector> _selector;
    QSharedPointer<Selector> _cinemaSelector;

    QSharedPointer<AccentPainter> _selectionAccentPainter;
    QSharedPointer<AccentPainter> _customDrawingsAccentPainter;
    QSharedPointer<AccentPainter> _cinemaAccentPainter;
};
