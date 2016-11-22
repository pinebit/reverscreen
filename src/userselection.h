#pragma once

#include <QObject>
#include <QRect>

// The object is tracking a user selection.
class UserSelection : public QObject
{
    Q_OBJECT

public:
    explicit UserSelection(QObject *parent = 0);

    // Clears the selection.
    void clear();

    // Adds a selection point, e.g. from a mouse move.
    void add(const QPoint& point);

    // Returns true if the selection objets is not clean.
    bool isSelected() const {
        return _isSelected;
    }

    // Returns all selection track.
    QList<QPoint> track() const {
        return _track;
    }

    // Evaluates the selection rectangle.
    // Returns null-rectangle if selection is not completed.
    QRect getRect() const;

signals:
    // Emitted on any selection state change.
    void signalSelectionChanged();

private:
    QList<QPoint> _track;
    bool _isSelected;
};
