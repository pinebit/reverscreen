#pragma once

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QGroupBox>


class WidgetUtils
{
public:
    static QWidget* createHSeparator();
    static QLabel* createInfoLabel(const QString& text);
    static void centerWindow(QWidget* widget);

private:
    WidgetUtils() {}
};
