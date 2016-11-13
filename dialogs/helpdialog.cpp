#include "dialogs/helpdialog.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSettings>
#include <QDir>

HelpDialog::HelpDialog(const QString& path, const QString& page, QWidget* parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Tool);
    setWindowTitle(tr("HOW TO USE REVERSCREEN"));

    QTextBrowser* textBrowser = new QTextBrowser;
    textBrowser->setSource(QUrl(path + page));
    textBrowser->setContextMenuPolicy(Qt::NoContextMenu);
    textBrowser->setTextInteractionFlags(Qt::NoTextInteraction);

    QCheckBox* checkBox = new QCheckBox(tr("Don't show this dialog again"));
    checkBox->setCheckState(Qt::Checked);
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxToggled(bool)));

    QPushButton* closeButton = new QPushButton(tr("Close"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(checkBox);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBrowser);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));
 
    QSettings settings;
    bool checked = settings.value("hideHelpDialog", true).toBool();
    checkBox->setCheckState(checked ? Qt::Checked : Qt::Unchecked);

    setFixedSize(450, 300);
    move(parent->geometry().center() - geometry().center());
}

void HelpDialog::showPage(const QString& path, const QString& page, QWidget* parent)
{
  HelpDialog *helpDialog = new HelpDialog(path, page, parent);
  helpDialog->exec();

}
void HelpDialog::checkBoxToggled(bool checked)
{
    QSettings settings;
    settings.setValue("hideHelpDialog", checked);
}
