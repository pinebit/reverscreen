#include <dock/accentwidget.h>
#include <controls/coloractionwidget.h>
#include <widgetfactory.h>

#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QColorDialog>


AccentWidget::AccentWidget(QWidget *parent)
    : QWidget(parent)
    , _accentMode(Rectangle)
    , _accentColor(Qt::black)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createInfoLabel(tr("Select an accent region, choose parameters\nand click Apply button.")));
    vlayout->addSpacing(8);

    // accent mode
    QGroupBox *groupBox = new QGroupBox(tr("Accent type:"));

    QRadioButton *rb1 = new QRadioButton(tr("Rectangle"));
    connect(rb1, &rb1->toggled, this, [this, rb1]() {
        if (rb1->isChecked()) {
            updateMode(Rectangle);
        }
    });
    QRadioButton *rb2 = new QRadioButton(tr("Cinema"));
    connect(rb2, &rb2->toggled, this, [this, rb2]() {
        if (rb2->isChecked()) {
            updateMode(Cinema);
        }
    });
    QRadioButton *rb3 = new QRadioButton(tr("Marker"));
    connect(rb3, &rb3->toggled, this, [this, rb3]() {
        if (rb3->isChecked()) {
            updateMode(Marker);
        }
    });
    QRadioButton *rb4 = new QRadioButton(tr("Hatching"));
    connect(rb4, &rb4->toggled, this, [this, rb4]() {
        if (rb4->isChecked()) {
            updateMode(Hatching);
        }
    });

    rb1->setChecked(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(rb1);
    vbox->addWidget(rb2);
    vbox->addWidget(rb3);
    vbox->addWidget(rb4);
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

    vlayout->addWidget(WidgetFactory::createInfoLabel(tr("Note: pressing Apply will merge the accent.\nYou cannot modify a merged accent.")));
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
    if (_accentColor == color) {
        return;
    }

    _accentColor = color;
    _colorAction->updateColor(_accentColor);

    emit signalAccentChanged();
}

void AccentWidget::updateMode(AccentMode accentMode)
{
    if (_accentMode == accentMode) {
        return;
    }

    _accentMode = accentMode;

    emit signalAccentChanged();
}

