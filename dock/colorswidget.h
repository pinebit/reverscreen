#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QColor>


class ColorsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorsWidget(QWidget *parent);

signals:
    void signalRemoveColor(QColor color);

public slots:
    void slotColorChanged(QColor color);

private slots:
    void slotCopyButton();
    void slotRemoveColorButton();

private:
    QWidget* createHSeparator() const;
    QLabel* createInfoLabel(const QString& text) const;

    QColor _color;
    QLabel* _colorLabel;
    QLabel* _hexLabel;
    QPushButton* _copyButton;
    QPushButton* _removeButton;
};

