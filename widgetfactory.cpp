#include "widgetfactory.h"

#include <QRadioButton>
#include <QVBoxLayout>


QWidget *WidgetFactory::createHSeparator()
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

QLabel *WidgetFactory::createInfoLabel(const QString &text)
{
    QLabel* infoLabel = new QLabel(text);
    infoLabel->setStyleSheet("QLabel { color : #555 }");
    return infoLabel;
}

QGroupBox *WidgetFactory::createOptionsBox(const QString& title, const QList<QString>& options)
{
    QGroupBox *groupBox = new QGroupBox(title);
    QVBoxLayout *vbox = new QVBoxLayout;

    bool selected = false;
    foreach (const QString& option, options) {
        QRadioButton *rb = new QRadioButton(option);
        if (!selected) {
            selected = true;
            rb->setChecked(true);
        }
        vbox->addWidget(rb);
    }

    groupBox->setLayout(vbox);

    return groupBox;
}

WidgetFactory::WidgetFactory()
{
}
