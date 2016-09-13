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

    _copyButton = new QPushButton(tr("Copy to clipboard"), this);
    connect(_copyButton, &_copyButton->clicked, this, &this->slotCopyButton);

    _removeButton = new QPushButton(tr("Remove color on image"), this);
    connect(_removeButton, &_copyButton->clicked, this, &this->slotRemoveColorButton);

    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createInfoLabel(tr("Click on the image to pick a color")));
    vlayout->addSpacing(8);
    vlayout->addLayout(hlayout);
    vlayout->addSpacing(8);
    vlayout->addWidget(_copyButton);
    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createHSeparator());
    vlayout->addSpacing(8);
    vlayout->addWidget(WidgetFactory::createInfoLabel(tr("This action removes all pixels\nmatching the selected color.")));
    vlayout->addWidget(_removeButton);
    vlayout->addSpacing(8);
    vlayout->addStretch();

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
}

void ColorsWidget::slotCopyButton()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(_hexLabel->text());
}

void ColorsWidget::slotRemoveColorButton()
{
    emit signalRemoveColor(_color);
}
