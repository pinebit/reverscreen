#pragma once

#include <QWidget>
#include <QLabel>
#include <QAction>


class ColorActionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorActionWidget(QColor initialColor, QAction* action = 0, QWidget *parent = 0);

    void updateColor(QColor newColor);

private:
    QColor  _color;
    QLabel* _colorLabel;
    QLabel* _hexLabel;
    QAction* _action;
};

