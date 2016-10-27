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

#include "awesomeservice.h"
#include "mainwindow.h"
#include "rsview.h"
#include "dock/colorswidget.h"
#include "dock/accentwidget.h"
#include "fullscreenselectiondialog.h"
#include "assistant/cvsnapassistant.h"
#include "accent/selectionaccentpainter.h"
#include "accent/rectangleaccentpainter.h"
#include "accent/cinemaaccentpainter.h"
#include "accent/hatchingaccentpainter.h"
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
}

void MainWindow::slotActionScreenshot()
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
    QRect cropRegion = _rsview->highlightedRegion();
    if (cropRegion.isEmpty() ||
        cropRegion.size() == QSize(1,1)) {
        QMessageBox::warning(this, tr("No region selected"), tr("Please use mouse to select a region to crop."));
        return;
    }

    QSize size = cropRegion.size();
    updateImage(_currentImage.copy(cropRegion));

    _statusbar->showMessage(tr("Image cropped. New size: %1x%2").arg(size.width()).arg(size.height()));
}

void MainWindow::slotSelectionStarted()
{
    if (_colorsDock->isVisible()) {
        _colorsWidget->setSelectedColor();
    }
}

void MainWindow::slotMouseMove(const QPoint &pos)
{
    if (_colorsDock->isVisible()) {
        QColor color(_currentImage.pixel(pos));
        _colorsWidget->setCurrentColor(color);
    }
}

void MainWindow::slotAccentChanged()
{
    _rsview->setAccentPainter(createAccentPainter());
    update();
}

void MainWindow::slotAccentApplied()
{
    QPixmap pm = QPixmap::fromImage(_currentImage);
    QPainter painter(&pm);

    QSharedPointer<AccentPainter> accent = createAccentPainter();
    accent->paint(&painter, pm.rect(), _rsview->highlightedRegion());

    updateImage(pm.toImage());

    _statusbar->showMessage(tr("Selected accent applied."));
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
            _accentDock->setVisible(false);
        }
        else {
            _colorsDock->setVisible(false);
            slotAccentChanged();
        }
    }
    else {
        if (dockWidget == _accentDock) {
            _rsview->setAccentPainter(createDefaultAccentPainter());
        }
    }
}

QSharedPointer<AccentPainter> MainWindow::createDefaultAccentPainter()
{
    return QSharedPointer<AccentPainter>(new SelectionAccentPainter(RegionColor, ShaderColor));
}

QSharedPointer<AccentPainter> MainWindow::createAccentPainter()
{
    QColor color = _accentWidget->accentColor();
    AccentPainter* accent = NULL;

    switch (_accentWidget->accentMode()) {
    case AccentWidget::Rectangle:
        accent = new RectangleAccentPainter(color, 3);
        break;
    case AccentWidget::Cinema:
        accent = new CinemaAccentPainter(color);
        break;
    case AccentWidget::Hatching:
        accent = new HatchingAccentPainter(color);
        break;
    default:
        qFatal("Unknown AccentMode");
    }

    return QSharedPointer<AccentPainter>(accent);
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
        viewRegionContext->setSelectedRegion(image.rect());
        viewRegionContext->setHighlightedRegion(highlightedRegion);
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
    setMinimumSize(QSize(400, 300));

    // font
    QFont font;
    font.setFamily(QStringLiteral("Calibri"));
    font.setPointSize(10);
    setFont(font);

    setUnifiedTitleAndToolBarOnMac(true);

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
    _statusbar->showMessage(tr("Ready."));

    // actions
    _actionPaste = new QAction(_awesome->icon(fa::paste), tr("Paste from clipboard"), this);
    _actionPaste->setShortcut(QKeySequence("Ctrl+V"));
    _actionOpen = new QAction(_awesome->icon(fa::filepictureo), tr("Open image file..."), this);
    _actionOpen->setShortcut(QKeySequence("Ctrl+O"));
    _actionCopy = new QAction(_awesome->icon(fa::copy), tr("Copy"), this);
    _actionCopy->setShortcut(QKeySequence("Ctrl+C"));
    _actionSave = new QAction(_awesome->icon(fa::save), tr("Save..."), this);
    _actionSave->setShortcut(QKeySequence("Ctrl+S"));
    _actionCrop = new QAction(_awesome->icon(fa::crop), tr("Crop"), this);
    _actionCrop->setShortcut(QKeySequence("Ctrl+X"));

    connect(_actionPaste, &QAction::triggered, this, &MainWindow::slotActionPaste);
    connect(_actionOpen, &QAction::triggered, this, &MainWindow::slotActionOpen);
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
    connect(_rsview, &RsView::signalMouseMove, this, &MainWindow::slotMouseMove);

    _scrollArea->setWidget(_rsview);

    setCentralWidget(_scrollArea);

    // docked widgets
    _colorsDock = new QDockWidget(tr("Colors"), this);
    _colorsWidget = new ColorsWidget(_colorsDock);
    setupDockWidget(_colorsDock, _awesome->icon(fa::eyedropper), _colorsWidget);

    _accentDock = new QDockWidget(tr("Accent"), this);
    _accentWidget = new AccentWidget(_accentDock);
    connect(_accentWidget, &AccentWidget::signalAccentChanged, this, &MainWindow::slotAccentChanged);
    connect(_accentWidget, &AccentWidget::signalAccentApplied, this, &MainWindow::slotAccentApplied);
    setupDockWidget(_accentDock, _awesome->icon(fa::lightbulbo), _accentWidget);

    // combo actions
    QMenu *newMenu = new QMenu(tr("Screenshot"));
    newMenu->menuAction()->setIcon(_awesome->icon(fa::cameraretro));
    newMenu->menuAction()->setShortcut(QKeySequence("Ctrl+N"));
    connect(newMenu->menuAction(), &QAction::triggered, this, &MainWindow::slotActionScreenshot);
    newMenu->addAction(_actionPaste);
    newMenu->addAction(_actionOpen);

    // toolbar
    _toolbar->addAction(newMenu->menuAction());
    _toolbar->addAction(_actionCopy);
    _toolbar->addAction(_actionSave);
    _toolbar->insertSeparator(0);
    _toolbar->addAction(_actionCrop);
    _toolbar->addAction(_colorsDock->toggleViewAction());
    // _toolbar->addAction(_accentDock->toggleViewAction());

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
