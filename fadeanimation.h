#pragma once

#include <QWidget>


class FadeAnimation
{
public:
    static void fadeIn(QWidget* widget);
    static void fadeOut(QWidget* widget);

private:
    FadeAnimation();
};
