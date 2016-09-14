#include "widgetfactory.h"

#include <QRadioButton>
#include <QVBoxLayout>


QWidget *WidgetFactory::createHSeparator()
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

QLabel *WidgetFactory::createInfoLabel(const QString &text)
{
    QLabel* infoLabel = new QLabel(text);
    infoLabel->setStyleSheet("QLabel { color : #555 }");
    return infoLabel;
}

WidgetFactory::WidgetFactory()
{
}
