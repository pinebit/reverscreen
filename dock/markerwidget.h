#pragma once

#include <QWidget>
#include <QList>

class ColorActionWidget;
class QToolButton;


class MarkerWidget : public QWidget
{
    Q_OBJECT

public:
    enum MarkerShape {
        Fill,
        Frame
    };

    Q_ENUM(MarkerShape)

    explicit MarkerWidget(QWidget *parent);

    QColor markerColor() const {
        return _currentColor;
    }

    MarkerShape markerShape() const {
        return _currentShape;
    }

signals:
    void signalUndo();
    void signalMarkerChanged();

private slots:
    void slotColorButtonToggled(QColor color);

private:
    void updateMode(MarkerShape markerShape);
    QToolButton* createColorSwitch(const QColor& color);

    QList<QToolButton*> _colorButtons;
    QColor _currentColor;
    MarkerShape _currentShape;
};
