#include "userselection.h"

UserSelection::UserSelection(QObject *parent)
    : QObject(parent)
    , _isSelected(false)
{
}

void UserSelection::clear()
{
    _track.clear();
    _isSelected = false;

    emit signalSelectionChanged();
}

void UserSelection::add(const QPoint &point)
{
    if (_track.isEmpty() || _track.last() != point) {
        _track << point;
    }

    _isSelected = _track.size() >= 2;

    emit signalSelectionChanged();
}

QRect UserSelection::getRect() const
{
    if (!_isSelected || _track.size() < 2) {
        return QRect();
    }

    return QRect(_track.first(), _track.last()).normalized();
}
