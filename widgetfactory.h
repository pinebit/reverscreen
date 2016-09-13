#pragma once

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QGroupBox>


class WidgetFactory
{
public:
    static QWidget* createHSeparator();
    static QLabel* createInfoLabel(const QString& text);
    static QGroupBox* createOptionsBox(const QString& title, const QList<QString>& options);

private:
    WidgetFactory();
};
