#include "dock/markerwidget.h"
#include "controls/coloractionwidget.h"
#include "widgetutils.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QColorDialog>
#include <QMetaEnum>


MarkerWidget::MarkerWidget(QWidget *parent)
    : QWidget(parent)
    , _markerColor(QColor::fromRgb(255, 255, 0, 100))
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetUtils::createInfoLabel(tr("Highlight image using the marker,\nand click Apply button.")));
    vlayout->addSpacing(8);

    // color selection
    QAction* changeAction = new QAction(tr("Change..."), this);
    connect(changeAction, &QAction::triggered, this, [this]() { this->slotSelectColorDialog(); });
    _colorAction = new ColorActionWidget(_markerColor, changeAction);

    vlayout->addWidget(_colorAction);
    vlayout->addSpacing(8);

    // apply button
    QPushButton* applyButton = new QPushButton(tr("Apply"));
    connect(applyButton, &QPushButton::clicked, this, &MarkerWidget::signalAccentApplied);

    vlayout->addWidget(applyButton);

    vlayout->addStretch();
}

void MarkerWidget::slotSelectColorDialog()
{
    QColorDialog dialog(_markerColor, this);
    if (dialog.exec() == QDialog::Accepted) {
        updateColor(dialog.selectedColor());
    }
}

void MarkerWidget::updateColor(QColor color)
{
    _markerColor = color;
    _markerColor.setAlpha(100);
    _colorAction->updateColor(_markerColor);

    emit signalAccentChanged();
}
