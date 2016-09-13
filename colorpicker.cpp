#include <colorpicker.h>

#include <QGuiApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QClipboard>
#include <QPalette>


ColorPicker::ColorPicker(QWidget *parent)
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

    QLabel* infoLabel = new QLabel(tr("Click on the image to pick a color"));
    infoLabel->setStyleSheet("QLabel { color : #555 }");

    vlayout->addSpacing(16);
    vlayout->addWidget(infoLabel);
    vlayout->addSpacing(16);
    vlayout->addLayout(hlayout);
    vlayout->addSpacing(8);
    vlayout->addWidget(_copyButton);
    vlayout->addWidget(_removeButton);
    vlayout->addStretch();

    slotColorChanged(Qt::black);
}

void ColorPicker::slotColorChanged(QColor color)
{
    _color = color;

    QPixmap icon(32, 16);
    icon.fill(color);

    _colorLabel->setPixmap(icon);
    _colorLabel->setFixedSize(icon.size());
    _hexLabel->setText(color.name().toUpper());
}

void ColorPicker::slotCopyButton()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(_hexLabel->text());
}

void ColorPicker::slotRemoveColorButton()
{
    emit signalRemoveColor(_color);
}
