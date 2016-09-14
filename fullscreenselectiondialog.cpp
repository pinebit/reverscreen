#include <QLabel>

#include <fullscreenselectiondialog.h>
#include <regionselector.h>
#include <fineselectionstrategy.h>
#include <snapselectionstrategy.h>
#include <CV/cvmodelbuilder.h>
#include <CV/cvmodel.h>


FullscreenSelectionDialog::FullscreenSelectionDialog(QWidget *parent, const QImage &image, AccentPainter* accentPainter)
    : QDialog(parent)
    , _accentPainter(accentPainter)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool | Qt::SubWindow);
    setWindowState(Qt::WindowFullScreen);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    QSharedPointer<SelectionStrategy> strategy(new FineSelectionStrategy());
    _regionSelector = new RegionSelector(this, image);
    _regionSelector->setSelectionStrategy(strategy, QCursor(Qt::WaitCursor));
    _regionSelector->setAccentPainter(accentPainter);

    connect(_regionSelector, &_regionSelector->signalSelectionFinished, this, this->slotSelectionFinished);
    connect(_regionSelector, &_regionSelector->signalSelectionCancelled, this, this->reject);

    _builder = new CvModelBuilder(this);
    connect(_builder, &_builder->signalBuildCompleted, this, &this->slotBuildCompleted);

    CvModelBuilderOptions options;
    _builder->buildAsync(image, options);
}

void FullscreenSelectionDialog::slotBuildCompleted(QSharedPointer<CvModel> model)
{
    QSharedPointer<SelectionStrategy> strategy(new SnapSelectionStrategy(model));
    _regionSelector->setSelectionStrategy(strategy, QCursor(Qt::CrossCursor));

    setCursor(Qt::ArrowCursor);
}

void FullscreenSelectionDialog::slotSelectionFinished()
{
    _selectedRegion = _regionSelector->selectedRegion();
    accept();
}
