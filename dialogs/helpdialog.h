#pragma once

#include <QDialog>
#include <QTextBrowser>
#include <QCheckBox>
#include <QPushButton>

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    HelpDialog(const QString& path, const QString& page, QWidget* parent);

    static void showPage(const QString& path, const QString& page, QWidget* parent);

private slots:
    void checkBoxToggled(bool);

private:
    QTextBrowser* textBrowser;
    QCheckBox* checkBox;
    QPushButton* closeButton;
};
