#include <dock/accentwidget.h>
#include <controls/coloractionwidget.h>
#include <widgetutils.h>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QColorDialog>
#include <QMetaEnum>


AccentWidget::AccentWidget(QWidget *parent)
    : QWidget(parent)
    , _accentMode(Rectangle)
    , _accentColor(Qt::blue)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetUtils::createInfoLabel(tr("Select an accent region, choose parameters\nand click Apply button.")));
    vlayout->addSpacing(8);

    // accent mode
    QGroupBox *groupBox = new QGroupBox(tr("Accent type:"));
    QVBoxLayout *vbox = new QVBoxLayout;

    QMetaEnum metaEnum = QMetaEnum::fromType<AccentMode>();
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);

        QRadioButton *rb = new QRadioButton(metaEnum.key(i));
        connect(rb, &rb->toggled, this, [this, rb, value]() {
            if (rb->isChecked()) {
                updateMode((AccentMode)value);
            }
        });

        if (i == 0) {
            rb->setChecked(true);
        }

        vbox->addWidget(rb);
    }

    groupBox->setLayout(vbox);

    vlayout->addWidget(groupBox);
    vlayout->addSpacing(8);

    // color selection
    QAction* changeAction = new QAction(tr("Change..."));
    connect(changeAction, &changeAction->triggered, this, [this]() { this->slotSelectColorDialog(); });
    _colorAction = new ColorActionWidget(_accentColor, changeAction);

    vlayout->addWidget(_colorAction);
    vlayout->addSpacing(8);

    // apply button
    QPushButton* applyButton = new QPushButton(tr("Apply"));
    connect(applyButton, &applyButton->clicked, this, &this->signalAccentApplied);

    vlayout->addWidget(WidgetUtils::createInfoLabel(tr("Note: pressing Apply will merge the accent.\nYou cannot modify a merged accent.")));
    vlayout->addSpacing(8);
    vlayout->addWidget(applyButton);

    vlayout->addStretch();
}

void AccentWidget::slotSelectColorDialog()
{
    QColorDialog dialog(_accentColor, this);
    if (dialog.exec() == QDialog::Accepted) {
        updateColor(dialog.selectedColor());
    }
}

void AccentWidget::updateColor(QColor color)
{
    _accentColor = color;
    _colorAction->updateColor(_accentColor);

    emit signalAccentChanged();
}

void AccentWidget::updateMode(AccentMode accentMode)
{
    _accentMode = accentMode;

    emit signalAccentChanged();
}

