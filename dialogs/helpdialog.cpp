#include "dialogs/helpdialog.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSettings>
#include <QDir>

inline QDir directoryOf(const QString &subdir)
{
    QDir dir(QApplication::applicationDirPath());
#if defined(Q_OS_WIN)
    if (dir.dirName().toLower() == "debug" || dir.dirName().toLower() == "release")
        dir.cdUp();
#elif defined(Q_OS_MAC)
    if (dir.dirName() == "MacOS") {
        dir.cdUp();
        dir.cdUp();
        dir.cdUp();
    }
#endif
    dir.cd(subdir);
    return dir;
}

HelpDialog::HelpDialog(const QString& path, const QString& page, QWidget* parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Tool);

    textBrowser = new QTextBrowser;

    checkBox = new QCheckBox(tr("Don't show this dialog"));
    checkBox->setCheckState(Qt::Checked);
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(checkBoxToggled(bool)));

    closeButton = new QPushButton(tr("Close"));
    closeButton->setShortcut(tr("Esc"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(checkBox);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBrowser);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    
    textBrowser->setSearchPaths(QStringList() << path);
    textBrowser->setSource(page);

    QSettings settings;
    if (settings.contains("showHelpDialog")){
        bool checked = settings.value("showHelpDialog").toBool();
        checkBox->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    }

    resize(500,400);
    move(parent->geometry().center() - geometry().center());
}

void HelpDialog::showPage(const QString& path, const QString& page, QWidget* parent)
{
  QString fullPath = qApp->applicationDirPath() + path;
  HelpDialog *helpDialog = new HelpDialog(fullPath, page, parent);
  helpDialog->exec();

}
void HelpDialog::checkBoxToggled(bool checked)
{
    QSettings settings;
    settings.setValue("showHelpDialog", checked);
}
