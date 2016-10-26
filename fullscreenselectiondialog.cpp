#include <QLabel>

#include "fullscreenselectiondialog.h"
#include "rsview.h"
#include "assistant/cvsnapassistant.h"
#include "cv/cvmodelbuilder.h"
#include "cv/cvmodel.h"

FullscreenSelectionDialog::FullscreenSelectionDialog(QWidget *parent, const QImage &image, QSharedPointer<AccentPainter> accentPainter)
    : QDialog(parent)
    , _accentPainter(accentPainter)
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool | Qt::SubWindow);
    setWindowState(Qt::WindowFullScreen);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    setCursor(Qt::WaitCursor);

    _rsview = new RsView(this, true);
    _rsview->setImage(image);
    _rsview->setAccentPainter(accentPainter);

    connect(_rsview, &RsView::signalSelectionFinished, this, &FullscreenSelectionDialog::slotSelectionFinished);
    connect(_rsview, &RsView::signalSelectionCancelled, this, &FullscreenSelectionDialog::reject);

    _builder = new CvModelBuilder(this);
    connect(_builder, &CvModelBuilder::signalBuildCompleted, this, &FullscreenSelectionDialog::slotBuildCompleted);

    CvModelBuilderOptions options;
    _builder->buildAsync(image, options);
}

QSharedPointer<RegionContext>& FullscreenSelectionDialog::getRegionContext(){
    return _rsview->getRegionContext();
}

void FullscreenSelectionDialog::slotBuildCompleted(QSharedPointer<CvModel> model){
    QSharedPointer<SnapAssistant> assistant(new CvSnapAssistant(model));
    _rsview->setSnapAssistant(assistant);

    setCursor(Qt::CrossCursor);
}

void FullscreenSelectionDialog::slotSelectionFinished() {
    accept();
}
