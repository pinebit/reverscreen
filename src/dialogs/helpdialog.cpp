#include "dialogs/helpdialog.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSettings>
#include <QDir>
#include <QDebug>

#define HIDE_DIALOG_SETTING_KEY  "hideHelpDialog"

HelpDialog::HelpDialog(const QString& path, const QString& page, QWidget* parent)
    : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint)
{
    setWindowTitle(tr("HELP"));

    QTextBrowser* textBrowser = new QTextBrowser;
    textBrowser->setSource(QUrl(path + page));
    textBrowser->setContextMenuPolicy(Qt::NoContextMenu);
    textBrowser->setTextInteractionFlags(Qt::NoTextInteraction);

    QCheckBox* checkBox = new QCheckBox(tr("Don't show this dialog again"));
    connect(checkBox, &QCheckBox::toggled, this, &HelpDialog::checkBoxToggled);
    checkBox->setCheckState(Qt::Checked);

    QPushButton* closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QPushButton::clicked, this, &HelpDialog::reject);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(checkBox);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBrowser);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setFixedSize(450, 300);
    move(parent->geometry().center() - geometry().center());
}

bool HelpDialog::canShowDialog()
{
    QSettings settings;
    return !settings.value(HIDE_DIALOG_SETTING_KEY, false).toBool();
}

void HelpDialog::checkBoxToggled(bool checked)
{
    QSettings settings;
    settings.setValue(HIDE_DIALOG_SETTING_KEY, checked);
}
