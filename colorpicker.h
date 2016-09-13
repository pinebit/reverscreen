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

public slots:
    void slotColorChanged(QColor color);

private slots:
    void slotCopyButton();

private:
    QLabel* _colorLabel;
    QLabel* _hexLabel;
    QPushButton* _copyButton;

};

