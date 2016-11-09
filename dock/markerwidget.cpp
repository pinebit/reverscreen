#include "dock/markerwidget.h"
#include "controls/coloractionwidget.h"
#include "widgetutils.h"
#include "awesomeservice.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMetaEnum>
#include <QRadioButton>
#include <QToolButton>


MarkerWidget::MarkerWidget(QWidget *parent)
    : QWidget(parent)
    , _currentShape(Fill)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetUtils::createInfoLabel(tr("Use marker to highlight a region of interest.")));
    vlayout->addSpacing(8);

    // undo button
    QPushButton* undoButton = new QPushButton(tr("Undo"));
    undoButton->setIcon(AwesomeService::awesome()->icon(fa::undo));
    undoButton->setMaximumWidth(100);
    connect(undoButton, &QPushButton::clicked, this, &MarkerWidget::signalUndo);

    vlayout->addWidget(undoButton);
    vlayout->addSpacing(8);

    // marker shape
    QGroupBox *groupBox = new QGroupBox(tr("Mode:"));
    QVBoxLayout *vbox = new QVBoxLayout;

    QMetaEnum metaEnum = QMetaEnum::fromType<MarkerShape>();
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        int value = metaEnum.value(i);

        QRadioButton *rb = new QRadioButton(metaEnum.key(i));
        connect(rb, &QRadioButton::toggled, this, [this, rb, value]() {
            if (rb->isChecked()) {
                updateMode((MarkerShape)value);
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

    // marker color
    groupBox = new QGroupBox(tr("Color:"));
    QHBoxLayout *hbox = new QHBoxLayout;

    hbox->addWidget(createColorSwitch(Qt::yellow));
    hbox->addWidget(createColorSwitch(Qt::green));
    hbox->addWidget(createColorSwitch(Qt::red));

    _colorButtons.first()->setChecked(true);

    groupBox->setLayout(hbox);

    vlayout->addWidget(groupBox);

    vlayout->addStretch();
}

void MarkerWidget::slotColorButtonToggled(QColor color)
{
    QToolButton* button = static_cast<QToolButton*>(sender());

    if (!button->isChecked()) {
        if (_currentColor == color) {
            button->setChecked(true);
        }
        return;
    }

    bool colorChanged = _currentColor != color;
    _currentColor = color;

    foreach (QToolButton* tb, _colorButtons) {
        if (tb != button) {
            tb->setChecked(false);
        }
    }

    if (colorChanged) {
        emit signalMarkerChanged();
    }
}

void MarkerWidget::updateMode(MarkerShape markerShape)
{
    _currentShape = markerShape;
    emit signalMarkerChanged();
}

QToolButton *MarkerWidget::createColorSwitch(const QColor &color)
{
    QPixmap pm(64, 64);
    pm.fill(color);

    QToolButton* button = new QToolButton;
    button->setIcon(pm);
    button->setIconSize(QSize(64, 64));
    button->setCheckable(true);
    button->setStyleSheet("QToolButton {"
                          "border: none;"
                          "}"
                          "QToolButton:checked {"
                          "border: 3px solid black;"
                          "background-color: palette(base);}");
    connect(button, &QToolButton::toggled, this, [color, this] () { this->slotColorButtonToggled(color); });

    _colorButtons << button;

    return button;
}

