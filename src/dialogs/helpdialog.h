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
    
    static bool canShowDialog();

private slots:
    void checkBoxToggled(bool);
};
