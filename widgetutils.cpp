#include "widgetutils.h"

#include <QRadioButton>
#include <QVBoxLayout>
#include <QStyle>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>


QWidget *WidgetUtils::createHSeparator()
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

QLabel *WidgetUtils::createInfoLabel(const QString &text)
{
    QLabel* infoLabel = new QLabel(text);
    infoLabel->setStyleSheet("QLabel { color : #555 }");
    return infoLabel;
}

void WidgetUtils::centerWindow(QWidget* widget)
{
    Q_ASSERT(widget != NULL);

    widget->setGeometry(
        QStyle::alignedRect(
            Qt::LayoutDirectionAuto,
            Qt::AlignCenter,
            widget->size(),
            qApp->desktop()->availableGeometry()
        )
    );
}

void WidgetUtils::fadeIn(QWidget *widget)
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

void WidgetUtils::fadeOut(QWidget *widget)
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

    animation->connect(animation, &QPropertyAnimation::finished, widget, &QWidget::hide);
}
