#pragma once

#include <QWidget>


class ColorActionWidget;

class AccentWidget : public QWidget
{
    Q_OBJECT

public:
    enum AccentMode {
        Rectangle,
        Cinema,
        Marker,
        Hatching
    };

    explicit AccentWidget(QWidget *parent);

    AccentMode accentMode() const {
        return _accentMode;
    }

    QColor accentColor() const {
        return _accentColor;
    }

signals:
    void signalAccentChanged();
    void signalAccentApplied();

private slots:
    void slotSelectColorDialog();

private:
    void updateColor(QColor color);
    void updateMode(AccentMode accentMode);

    AccentMode _accentMode;
    QColor  _accentColor;
    ColorActionWidget* _colorAction;
};
