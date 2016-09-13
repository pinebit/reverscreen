#include <dock/cropwidget.h>
#include <widgetfactory.h>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>


CropWidget::CropWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createInfoLabel(tr("Select a region you want to keep.")));
    vlayout->addSpacing(8);

    QGroupBox *group = WidgetFactory::createOptionsBox(tr("Snap mode:"), QStringList() << "Precise" << "Assisted");
    vlayout->addWidget(group);

    vlayout->addSpacing(8);

    QPushButton* cropButton = new QPushButton(tr("Crop to selection"), this);
    connect(cropButton, &cropButton->clicked, this, &this->signalCrop);

    vlayout->addWidget(cropButton);
    vlayout->addStretch();
}
