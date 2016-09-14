#include <dock/accentwidget.h>
#include <widgetfactory.h>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>


AccentWidget::AccentWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createInfoLabel(tr("Select a region you want to highlight.")));
    vlayout->addSpacing(8);

    vlayout->addStretch();
}
