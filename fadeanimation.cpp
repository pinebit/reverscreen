#include <fadeanimation.h>

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>


void FadeAnimation::fadeIn(QWidget *widget)
{
    Q_ASSERT(widget != NULL);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);

    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(600);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->setEasingCurve(QEasingCurve::InBack);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void FadeAnimation::fadeOut(QWidget *widget)
{
    Q_ASSERT(widget != NULL);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);

    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(600);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->setEasingCurve(QEasingCurve::OutBack);
    animation->start(QPropertyAnimation::DeleteWhenStopped);

    animation->connect(animation, &animation->finished, widget, widget->hide);
}

FadeAnimation::FadeAnimation()
{
}
