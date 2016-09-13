#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QColor>


class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent);

signals:
    void signalRemoveColor(QColor color);

public slots:
    void slotColorChanged(QColor color);

private slots:
    void slotCopyButton();
    void slotRemoveColorButton();

private:
    QColor _color;
    QLabel* _colorLabel;
    QLabel* _hexLabel;
    QPushButton* _copyButton;
    QPushButton* _removeButton;
};

