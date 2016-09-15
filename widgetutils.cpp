#include "widgetutils.h"

#include <QRadioButton>
#include <QVBoxLayout>
#include <QStyle>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QApplication>


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
