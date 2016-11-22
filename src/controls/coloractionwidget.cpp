#include "controls/coloractionwidget.h"

#include <QPixmap>
#include <QHBoxLayout>
#include <QToolButton>

static const int ColorIconWidth = 32;
static const int ColorIconHeight = 16;


ColorActionWidget::ColorActionWidget(QColor initialColor, QAction* action, QWidget *parent)
    : QWidget(parent)
{
    _action = action;

    QHBoxLayout* layout = new QHBoxLayout();
    _colorLabel = new QLabel();
    _colorLabel->setFrameStyle(QFrame::Box);
    _hexLabel = new QLabel();

    updateColor(initialColor);

    layout->addWidget(_colorLabel);
    layout->addWidget(_hexLabel);

    if (_action) {
        QToolButton* button = new QToolButton();
        button->setDefaultAction(_action);
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        layout->addWidget(button);
    }

    setLayout(layout);
}

void ColorActionWidget::updateColor(QColor newColor)
{
    _color = newColor;

    QPixmap icon(ColorIconWidth, ColorIconHeight);
    icon.fill(_color);

    _colorLabel->setFixedSize(icon.size());
    _colorLabel->setPixmap(icon);
    _hexLabel->setText(_color.name().toUpper());
}
