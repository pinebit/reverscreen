#include "dock/colorswidget.h"
#include "controls/coloractionwidget.h"
#include "widgetutils.h"
#include "awesomeservice.h"

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
    QListWidgetItem *item = new QListWidgetItem(_colorList);
    _colorList->addItem(item);

    QAction* copyAction = new QAction(AwesomeService::awesome()->icon(fa::copy), tr("Copy"), this);
    ColorActionWidget* copyColorAction = new ColorActionWidget(_color, copyAction);
    connect(copyAction, &QAction::triggered, this, [=]() { this->copyColor(copyColorAction->color()); });
    item->setSizeHint(copyColorAction->minimumSizeHint());

    _colorList->setItemWidget(item, copyColorAction);
}

void ColorsWidget::clearColors()
{
    _colorList->clear();
}

void ColorsWidget::copyColor(QColor color)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(color.name().toUpper());
}

