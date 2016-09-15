#include <dock/colorswidget.h>
#include <controls/coloractionwidget.h>
#include <widgetutils.h>

#include <QGuiApplication>
#include <QVBoxLayout>
#include <QClipboard>
#include <QPalette>


ColorsWidget::ColorsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetUtils::createInfoLabel(tr("Current color:")));
    vlayout->addSpacing(8);

    _colorAction = new ColorActionWidget(Qt::black);
    vlayout->addWidget(_colorAction);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetUtils::createHSeparator());

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetUtils::createInfoLabel(tr("Selected colors:")));

    _colorList = new QListWidget(this);
    vlayout->addWidget(_colorList);

    setCurrentColor(Qt::black);
}

void ColorsWidget::setCurrentColor(QColor color)
{
    _color = color;
    _colorAction->updateColor(_color);
}

void ColorsWidget::setSelectedColor()
{
    QAction* copyAction = new QAction(tr("Copy"));
    connect(copyAction, &copyAction->triggered, this, [=]() { this->copyColor(_color); });

    QListWidgetItem *item = new QListWidgetItem(_colorList);
    _colorList->addItem(item);

    ColorActionWidget* copyColorAction = new ColorActionWidget(_color, copyAction);
    item->setSizeHint(copyColorAction->minimumSizeHint());
    _colorList->setItemWidget(item, copyColorAction);
}

void ColorsWidget::copyColor(QColor color)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(color.name().toUpper());
}

