#pragma once

#include <QWidget>

class CropWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CropWidget(QWidget *parent);

signals:
    void signalCrop();
    void signalSetPreciseSnap();
    void signalSetAssistedSnap();
};
