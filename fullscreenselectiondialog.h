#pragma once

#include <QDialog>
#include <QImage>

class RegionSelector;

class FullscreenSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    FullscreenSelectionDialog(QWidget* parent, const QImage& image);

private:
    RegionSelector* _regionSelector;
};

