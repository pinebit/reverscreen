#pragma once

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QGroupBox>


class WidgetFactory
{
public:
    static QWidget* createHSeparator();
    static QLabel* createInfoLabel(const QString& text);

private:
    WidgetFactory();
};
