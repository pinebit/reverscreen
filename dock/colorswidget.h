#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QColor>
#include <QListWidget>


class ColorActionWidget;

class ColorsWidget : public QWidget
{
    Q_OBJECT

public:
    ColorsWidget(QWidget *parent);

    void setCurrentColor(QColor color);
    void setSelectedColor();

private:
    QWidget* createHSeparator() const;
    QLabel* createInfoLabel(const QString& text) const;
    void copyColor(QColor color);

    QColor _color;
    QListWidget* _colorList;
    ColorActionWidget* _colorAction;
};

