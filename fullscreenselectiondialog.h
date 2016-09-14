#pragma once

#include <QDialog>
#include <QImage>
#include <QSharedPointer>

class RegionSelector;
class CvModelBuilder;
class CvModel;
class AccentPainter;

class FullscreenSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    FullscreenSelectionDialog(QWidget* parent, const QImage& image, AccentPainter* accentPainter);

    QRect selectedRegion() const {
        return _selectedRegion;
    }

private slots:
    void slotBuildCompleted(QSharedPointer<CvModel> model);
    void slotSelectionFinished();

private:
    RegionSelector* _regionSelector;
    CvModelBuilder* _builder;
    QRect           _selectedRegion;
    AccentPainter*  _accentPainter;
};

