#pragma once

#include <QWidget>


class ColorActionWidget;

class MarkerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarkerWidget(QWidget *parent);

    QColor markerColor() const {
        return _markerColor;
    }

signals:
    void signalAccentChanged();
    void signalAccentApplied();

private slots:
    void slotSelectColorDialog();

private:
    void updateColor(QColor color);

    QColor  _markerColor;
    ColorActionWidget* _colorAction;
};
