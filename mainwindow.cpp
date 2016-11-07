#include <QMessageBox>
#include <QImage>
#include <QScreen>
#include <QClipboard>
#include <QImageWriter>
#include <QImageReader>
#include <QDir>
#include <QStandardPaths>
#include <QStyle>
#include <QTime>
#include <QGuiApplication>
#include <QApplication>
#include <QLabel>
#include <QSharedPointer>
#include <QListWidget>
#include <QBitmap>
#include <QMenu>
#include <QDesktopWidget>
#include <QDebug>
#include <QMimeData>
#include <QSettings>

#include "awesomeservice.h"
#include "mainwindow.h"
#include "rsview.h"
#include "dock/colorswidget.h"
#include "dock/markerwidget.h"
#include "fullscreenselectiondialog.h"
#include "assistant/cvsnapassistant.h"
#include "accent/selectionaccentpainter.h"
#include "accent/rectangleaccentpainter.h"
#include "accent/cinemaaccentpainter.h"
#include "accent/markeraccentpainter.h"
#include "widgetutils.h"
#include "cv/cvmodelbuilder.h"

static const QColor RegionColor = Qt::red;
static const QColor ShaderColor = QColor::fromRgba(0x50a0a0a0);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _modelBuilder(new CvModelBuilder(this))
{
    connect(_modelBuilder, &CvModelBuilder::signalBuildCompleted, this, &MainWindow::slotBuildCompleted);

    setupUi();
    enableDisableUi();

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::slotActionCapture()
{
    hide();
    delay(300);

    QScreen *screen = QGuiApplication::primaryScreen();
    _currentImage = screen->grabWindow(QApplication::desktop()->winId()).toImage();

    FullscreenSelectionDialog dialog(this, _currentImage, createDefaultAccentPainter());
    if (dialog.exec() == QDialog::Accepted) {
        updateImage(dialog.getRegionContext(), dialog.usedHighlightedRegion());

        _statusbar->showMessage(tr("A screen region is captured."));
    }

    show();
}

void MainWindow::slotActionPaste()
{
    QClipboard *clipboard = QApplication::clipboard();
    QImage image = clipboard->image();
    if (image.isNull()) {
        _statusbar->showMessage(tr("Clipboard has no image."));
    }
    else {
        updateImage(image);
    }
}

void MainWindow::slotActionCopy()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(_currentImage);

    _statusbar->showMessage(tr("Copied to the clipboard."));
}

void MainWindow::slotActionSave()
{
    QFileDialog dialog(this, tr("Save Image As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveImage(dialog.selectedFiles().first())) {}
}

void MainWindow::slotActionOpen()
{
    QFileDialog dialog(this, tr("Open Image"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !openImage(dialog.selectedFiles().first())) {}
}

void MainWindow::slotActionCrop()
{
    QRect cropRegion = _rsview->selectedRegion();
    if (!RegionContext::isValidRegion(cropRegion)) {
        QMessageBox::warning(this, tr("No region selected"), tr("Please select a region to crop."));
        return;
    }

    QSize size = cropRegion.size();
    updateImage(_currentImage.copy(cropRegion));

    _statusbar->showMessage(tr("Cropped image size: %1x%2").arg(size.width()).arg(size.height()));
}

void MainWindow::slotSelectionStarted()
{
    if (_colorsDock->isVisible()) {
        _colorsWidget->setSelectedColor();
    }

    _actionCrop->setEnabled(true);
}

void MainWindow::slotSelectionCancelled()
{
    _actionCrop->setEnabled(false);
}

void MainWindow::slotMouseMove(const QPoint &pos)
{
    if (_colorsDock->isVisible()) {
        QColor color(_currentImage.pixel(pos));
        _colorsWidget->setCurrentColor(color);
    }

    bool hasSelection = _rsview && RegionContext::isValidRegion(_rsview->selectedRegion());
    _actionCrop->setEnabled(hasSelection);
}

void MainWindow::slotResetMarker()
{
    /*
    QPixmap pm = QPixmap::fromImage(_currentImage);
    QPainter painter(&pm);

    QSharedPointer<AccentPainter> accent = createMarkerAccentPainter();
    accent->paint(&painter, pm.rect(), _rsview->selectedRegion());

    updateImage(pm.toImage());

    _statusbar->showMessage(tr("Selected accent applied."));
    */
}

void MainWindow::slotBuildCompleted(QSharedPointer<CvModel> model)
{
    QSharedPointer<SnapAssistant> assistant(new CvSnapAssistant(model));
    _rsview->setSnapAssistant(assistant);
    _rsview->setCursor(Qt::CrossCursor);

    setCursor(Qt::ArrowCursor);

    enableDisableUi();
    show();
}

void MainWindow::handleDockWidgetVisibityChange(QDockWidget *dockWidget)
{
    if (dockWidget->isVisible()) {
        if (_colorsDock == dockWidget) {
            _markerDock->setVisible(false);
        }
        else {
            _colorsDock->setVisible(false);
            _rsview->setAccentPainter(createMarkerAccentPainter());
            update();
        }
    }
    else {
        if (dockWidget == _markerDock) {
            _rsview->setAccentPainter(createDefaultAccentPainter());
        }
    }
}

QSharedPointer<AccentPainter> MainWindow::createDefaultAccentPainter()
{
    return QSharedPointer<AccentPainter>(new SelectionAccentPainter(RegionColor, ShaderColor));
}

QSharedPointer<AccentPainter> MainWindow::createMarkerAccentPainter()
{
    return QSharedPointer<AccentPainter>(new MarkerAccentPainter(Qt::yellow));
}

bool MainWindow::saveImage(const QString &fileName)
{
    QImageWriter writer(fileName);
    if (!writer.write(_currentImage)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot save %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }

    const QString message = tr("Saved \"%1\"").arg(QDir::toNativeSeparators(fileName));
    _statusbar->showMessage(message);

    return true;
}

bool MainWindow::openImage(const QString &fileName)
{
    QImageReader reader(fileName);
    QImage image = reader.read();
    if (image.isNull()) {
        return false;
    }

    const QString message = tr("Opened \"%1\"").arg(QDir::toNativeSeparators(fileName));
    _statusbar->showMessage(message);

    updateImage(image);
    return true;
}

void MainWindow::initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");
    if (acceptMode == QFileDialog::AcceptSave) {
        dialog.setDefaultSuffix("png");
    }

    dialog.setAcceptMode(acceptMode);
}

void MainWindow::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs(millisecondsToWait);
    while( QTime::currentTime() < dieTime ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MainWindow::updateImage(const QImage& image)
{
    setCursor(Qt::WaitCursor);

    if (image.format() != QImage::Format_RGBA8888) {
       _currentImage = image.convertToFormat(QImage::Format_RGBA8888);
    } else {
        _currentImage = image;
    }

    _rsview->setImage(_currentImage);
    _modelBuilder->buildAsync(_currentImage, CvModelBuilderOptions());
}

void MainWindow::updateImage(const QSharedPointer<RegionContext>& regionContext, bool bHighlightedRegion)
{
    setCursor(Qt::WaitCursor);

    QRect selectedRegion = regionContext->selectedRegion();
    QRect highlightedRegion = regionContext->highlightedRegion();
    if (selectedRegion.isEmpty() && highlightedRegion.isEmpty()) {
        return;
    }

    QRect cropRegion = (bHighlightedRegion) ? highlightedRegion : selectedRegion;

    QImage image = _currentImage.copy(cropRegion);
    if (image.format() != QImage::Format_RGBA8888) {
        _currentImage = image.convertToFormat(QImage::Format_RGBA8888);
    } else {
        _currentImage = image;
    }
    _rsview->setImage(_currentImage);

    if (!bHighlightedRegion) {
        // Moves the highlightedRegion
        highlightedRegion.translate(-selectedRegion.topLeft());

        QSharedPointer<RegionContext>& viewRegionContext = _rsview->getRegionContext();
        viewRegionContext->setSelectedRegion(highlightedRegion);
        viewRegionContext->setHighlightedRegion(QRect());
        update();
    }

    _colorsWidget->clearColors();
    _modelBuilder->buildAsync(_currentImage, CvModelBuilderOptions());
}

void MainWindow::enableDisableUi()
{
    bool hasImage = !_currentImage.isNull();
    bool skipFirst = true;

    foreach (QAction* action, _toolbar->actions()) {
        if (skipFirst) {
            skipFirst = false;
            continue;
        }
        action->setEnabled(hasImage);
    }
}

void MainWindow::setupUi()
{
    // awesome font
    AwesomeService::create(this);
    _awesome = AwesomeService::awesome();

    // geometry & title
    setWindowTitle(tr("REVERSCREEN"));
    setMinimumSize(QSize(600, 400));

    // font
    QFont font;
    font.setFamily(QStringLiteral("Calibri"));
    font.setPointSize(10);
    setFont(font);

    setUnifiedTitleAndToolBarOnMac(true);
    setAcceptDrops(true);

    // toolbar
    _toolbar = new QToolBar(this);
    _toolbar->setFont(font);
    _toolbar->setMovable(false);
    _toolbar->setFloatable(false);
    _toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(Qt::TopToolBarArea, _toolbar);

    // statusbar
    _statusbar = new QStatusBar(this);
    _statusbar->setFont(font);
    setStatusBar(_statusbar);
    _statusbar->showMessage(tr("Click Capture, paste from the clipboard or drag and drop an image here."));

    // actions
    _actionCapture = new QAction(_awesome->icon(fa::cameraretro), tr("Capture"), this);
    _actionCapture->setShortcut(QKeySequence("Ctrl+N"));
    _actionPaste = new QAction(_awesome->icon(fa::paste), tr("Paste from clipboard"), this);
    _actionPaste->setShortcut(QKeySequence("Ctrl+V"));
    _actionCopy = new QAction(_awesome->icon(fa::copy), tr("Copy"), this);
    _actionCopy->setShortcut(QKeySequence("Ctrl+C"));
    _actionSave = new QAction(_awesome->icon(fa::save), tr("Save..."), this);
    _actionSave->setShortcut(QKeySequence("Ctrl+S"));
    _actionCrop = new QAction(_awesome->icon(fa::crop), tr("Crop"), this);
    _actionCrop->setShortcut(QKeySequence("Ctrl+X"));

    addAction(_actionPaste);

    connect(_actionCapture, &QAction::triggered, this, &MainWindow::slotActionCapture);
    connect(_actionPaste, &QAction::triggered, this, &MainWindow::slotActionPaste);
    connect(_actionCopy, &QAction::triggered, this, &MainWindow::slotActionCopy);
    connect(_actionSave, &QAction::triggered, this, &MainWindow::slotActionSave);
    connect(_actionCrop, &QAction::triggered, this, &MainWindow::slotActionCrop);

    // central widget
    _scrollArea = new QScrollArea(this);
    _scrollArea->setBackgroundRole(QPalette::BrightText);
    _scrollArea->setAlignment(Qt::AlignCenter);
    _scrollArea->setFrameStyle(QFrame::NoFrame);

    _scrollArea->viewport()->installEventFilter(this);

    _rsview = new RsView(_scrollArea, false);
    _rsview->setAccentPainter(createDefaultAccentPainter());
    connect(_rsview, &RsView::signalSelectionStarted, this, &MainWindow::slotSelectionStarted);
    connect(_rsview, &RsView::signalSelectionCancelled, this, &MainWindow::slotSelectionCancelled);
    connect(_rsview, &RsView::signalMouseMove, this, &MainWindow::slotMouseMove);

    _scrollArea->setWidget(_rsview);

    setCentralWidget(_scrollArea);

    // docked widgets
    _colorsDock = new QDockWidget(tr("Colors"), this);
    _colorsWidget = new ColorsWidget(_colorsDock);
    setupDockWidget(_colorsDock, _awesome->icon(fa::eyedropper), _colorsWidget);

    _markerDock = new QDockWidget(tr("Marker"), this);
    _markerWidget = new MarkerWidget(_markerDock);
    connect(_markerWidget, &MarkerWidget::signalResetMarker, this, &MainWindow::slotResetMarker);
    setupDockWidget(_markerDock, _awesome->icon(fa::lightbulbo), _markerWidget);

    // toolbar
    _toolbar->addAction(_actionCapture);
    _toolbar->addAction(_actionCopy);
    _toolbar->addAction(_actionSave);
    _toolbar->insertSeparator(0);
    _toolbar->addAction(_actionCrop);
    _toolbar->addAction(_colorsDock->toggleViewAction());
    _toolbar->addAction(_markerDock->toggleViewAction());

    WidgetUtils::centerWindow(this);
}

void MainWindow::setupDockWidget(QDockWidget *dockWidget, QIcon icon, QWidget *contentWidget)
{
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockWidget->setFeatures(QDockWidget::DockWidgetClosable);
    dockWidget->setVisible(false);
    dockWidget->setWidget(contentWidget);
    dockWidget->toggleViewAction()->setIcon(icon);

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    connect(dockWidget, &QDockWidget::visibilityChanged, this, [this, dockWidget]() { handleDockWidgetVisibityChange(dockWidget); });
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == _scrollArea->viewport()){
        if (event->type() == QEvent::Wheel) {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            wheelEvent->ignore();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() || event->mimeData()->hasImage()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasImage()) {
        event->acceptProposedAction();
        QImage image = qvariant_cast<QImage>(event->mimeData()->imageData());
        updateImage(image);
        _statusbar->showMessage(tr("New image has been received."));
        return;
    }

    foreach (const QUrl& url, event->mimeData()->urls()) {
        if (!url.isLocalFile()) {
            continue;
        }

        if (openImage(url.toLocalFile())) {
            event->acceptProposedAction();
            return;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    QMainWindow::closeEvent(event);
}
