#include "dock/markerwidget.h"
#include "controls/coloractionwidget.h"
#include "widgetutils.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>


MarkerWidget::MarkerWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetUtils::createInfoLabel(tr("Drag mouse over the area you want to marker.\nClick Reset button to clear marker.")));
    vlayout->addSpacing(8);

    // reset button
    QPushButton* resetButton = new QPushButton(tr("Reset"));
    connect(resetButton, &QPushButton::clicked, this, &MarkerWidget::signalResetMarker);

    vlayout->addWidget(resetButton);
    vlayout->addStretch();
}
