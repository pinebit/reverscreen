#include <dock/colorswidget.h>
#include <widgetfactory.h>

#include <QGuiApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QClipboard>
#include <QPalette>


ColorsWidget::ColorsWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* vlayout = new QVBoxLayout(this);
    QHBoxLayout* hlayout = new QHBoxLayout(0);

    _colorLabel = new QLabel("", this);
    _colorLabel->setFrameStyle(QFrame::Box);
    _hexLabel = new QLabel("", this);
    hlayout->addWidget(_colorLabel);
    hlayout->addWidget(_hexLabel);

    _colorList = new QListWidget(this);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createInfoLabel(tr("Current color:")));
    vlayout->addSpacing(8);
    vlayout->addLayout(hlayout);
    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createHSeparator());
    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createInfoLabel(tr("Selected colors:")));
    vlayout->addWidget(_colorList);

    slotColorChanged(Qt::black);
}

void ColorsWidget::slotColorChanged(QColor color)
{
    _color = color;

    QPixmap icon(32, 16);
    icon.fill(color);

    _colorLabel->setPixmap(icon);
    _colorLabel->setFixedSize(icon.size());
    _hexLabel->setText(color.name().toUpper());

    QHBoxLayout* hlayout = new QHBoxLayout(0);
    QLabel* cl = new QLabel();
    cl->setFixedSize(icon.size());
    cl->setPixmap(icon);
    cl->setFrameStyle(QFrame::Box);
    QLabel* hl = new QLabel(color.name().toUpper());
    QPushButton* cb = new QPushButton(tr("Copy"));
    connect(cb, &cb->clicked, this, [=]() { this->copyColor(color); });

    hlayout->addWidget(cl);
    hlayout->addWidget(hl);
    hlayout->addWidget(cb);

    QListWidgetItem *item = new QListWidgetItem(_colorList);
    _colorList->addItem(item);
    QWidget *w = new QWidget(this);
    w->setLayout(hlayout);
    item->setSizeHint(w->minimumSizeHint());
    _colorList->setItemWidget(item, w);
}

void ColorsWidget::copyColor(QColor color)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(color.name().toUpper());
}

