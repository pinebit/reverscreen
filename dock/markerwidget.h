#pragma once

#include <QWidget>


class ColorActionWidget;

class MarkerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarkerWidget(QWidget *parent);

signals:
    void signalResetMarker();
};
