#include <QLabel>

#include "fullscreenselectiondialog.h"
#include "rsview.h"
#include "cv/cvmodelbuilder.h"
#include "cv/cvmodel.h"
#include "userselection.h"
#include "renderer/cvselectionrenderer.h"

FullscreenSelectionDialog::FullscreenSelectionDialog(QWidget *parent, const QImage &image, QSharedPointer<AccentPainter> accentPainter)
    : QDialog(parent)
    , _accentPainter(accentPainter)
{
    setWindowFlags(/* Qt::WindowStaysOnTopHint | */ Qt::FramelessWindowHint | Qt::Tool | Qt::SubWindow);
    setWindowState(Qt::WindowFullScreen);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    setCursor(Qt::WaitCursor);

    _rsview = new RsView(this);
    _rsview->setImage(image);
    _rsview->setSelectionAccentPainter(accentPainter);

    connect(_rsview, &RsView::signalSelectionFinished, this, &FullscreenSelectionDialog::slotSelectionFinished);
    connect(_rsview, &RsView::signalSelectionCancelled, this, &FullscreenSelectionDialog::reject);

    _builder = new CvModelBuilder(this);
    connect(_builder, &CvModelBuilder::signalBuildCompleted, this, &FullscreenSelectionDialog::slotBuildCompleted);

    CvModelBuilderOptions options;
    _builder->buildAsync(image, options);
}

QRect FullscreenSelectionDialog::getSelectedRect() const
{
    return _rsview->userSelection()->getRect();
}

void FullscreenSelectionDialog::slotBuildCompleted(QSharedPointer<CvModel> model)
{
    setCursor(Qt::CrossCursor);
    _rsview->setSelectionRenderer(QSharedPointer<SelectionRenderer>(new CvSelectionRenderer(model)));
}

void FullscreenSelectionDialog::slotSelectionFinished()
{
    accept();
}
