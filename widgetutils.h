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
    static void fadeIn(QWidget* widget);
    static void fadeOut(QWidget* widget);

private:
    WidgetUtils() {}
};
