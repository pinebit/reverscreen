#include <QLabel>

#include <fullscreenselectiondialog.h>
#include <rsview.h>
#include <assistant/cvsnapassistant.h>
#include <CV/cvmodelbuilder.h>
#include <CV/cvmodel.h>


FullscreenSelectionDialog::FullscreenSelectionDialog(QWidget *parent, const QImage &image, QSharedPointer<AccentPainter> accentPainter)
    : QDialog(parent)
    , _accentPainter(accentPainter)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool | Qt::SubWindow);
    setWindowState(Qt::WindowFullScreen);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    setCursor(Qt::WaitCursor);

    _rsview = new RsView(this);
    _rsview->setImage(image);
    _rsview->setAccentPainter(accentPainter);

    connect(_rsview, &_rsview->signalSelectionFinished, this, this->slotSelectionFinished);
    connect(_rsview, &_rsview->signalSelectionCancelled, this, this->reject);

    _builder = new CvModelBuilder(this);
    connect(_builder, &_builder->signalBuildCompleted, this, &this->slotBuildCompleted);

    CvModelBuilderOptions options;
    _builder->buildAsync(image, options);
}

void FullscreenSelectionDialog::slotBuildCompleted(QSharedPointer<CvModel> model)
{
    QSharedPointer<SnapAssistant> assistant(new CvSnapAssistant(model));
    _rsview->setSnapAssistant(assistant);

    setCursor(Qt::CrossCursor);
}

void FullscreenSelectionDialog::slotSelectionFinished()
{
    _selectedRegion = _rsview->selectedRegion();
    accept();
}
