#include <QMessageBox>
#include <QImage>
#include <QScreen>
#include <QClipboard>
#include <QImageWriter>
#include <QImageReader>
#include <QDir>
#include <QStandardPaths>
#include <QStyle>
#include <QDesktopWidget>
#include <QTime>
#include <QGuiApplication>
#include <QApplication>
#include <QLabel>
#include <QSharedPointer>
#include <QListWidget>
#include <QBitmap>

#include <QtAwesome.h>

#include <mainwindow.h>
#include <regionselector.h>
#include <dock/colorswidget.h>
#include <dock/cropwidget.h>
#include <fullscreenselectiondialog.h>
#include <fineselectionstrategy.h>
#include <snapselectionstrategy.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

void MainWindow::slotActionNew()
{
    hide();
    delay(300);

    QScreen *screen = QGuiApplication::primaryScreen();
    _currentImage = screen->grabWindow(QApplication::desktop()->winId()).toImage();

    FullscreenSelectionDialog dialog(this, _currentImage);
    if (dialog.exec() == QDialog::Accepted) {
        updateImage(_currentImage.copy(dialog.selectedRegion()));
        _statusbar->showMessage(tr("A screen region is captured."));
    }

    show();
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
    if (_regionSelector->selectedRegion().isEmpty() ||
        _regionSelector->selectedRegion().size() == QSize(1,1)) {
        QMessageBox::warning(this, tr("No region selected"), tr("Please use mouse to select a region to crop."));
        return;
    }

    QSize size = _regionSelector->selectedRegion().size();
    updateImage(_currentImage.copy(_regionSelector->selectedRegion()));

    _statusbar->showMessage(tr("Image cropped. New size: %1x%2").arg(size.width()).arg(size.height()));
}

void MainWindow::slotSelectionStarted()
{
    if (_colorsDock->isVisible()) {
        QRect region = _regionSelector->selectedRegion();
        QColor color = _currentImage.pixelColor(region.left(), region.top());
        emit signalColorPicked(color);
    }
}

void MainWindow::slotRemoveColor(QColor color)
{
    QPixmap pixmap = QPixmap::fromImage(_currentImage);
    QBitmap mask = pixmap.createMaskFromColor(color.rgb());
    pixmap.setMask(mask);
    updateImage(pixmap.toImage());
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

void MainWindow::centerWindow()
{
    setGeometry(
        QStyle::alignedRect(
            Qt::LayoutDirectionAuto,
            Qt::AlignCenter,
            size(),
            qApp->desktop()->availableGeometry()
        )
    );
}

void MainWindow::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime ) {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void MainWindow::updateImage(const QImage& image)
{
    QSharedPointer<SelectionStrategy> strategy(new FineSelectionStrategy());
    _regionSelector = new RegionSelector(_scrollArea, image);
    _regionSelector->setSelectionStrategy(strategy, QCursor(Qt::CrossCursor));
    _scrollArea->setWidget(_regionSelector);

    connect(_regionSelector, &_regionSelector->signalSelectionStarted, this, &this->slotSelectionStarted);

    ColorsWidget* colorPicker = new ColorsWidget(_colorsDock);
    connect(this, &this->signalColorPicked, colorPicker, &colorPicker->slotColorChanged);
    connect(colorPicker, &colorPicker->signalRemoveColor, this, &this->slotRemoveColor);
    _colorsDock->setWidget(colorPicker);

    setCursor(Qt::ArrowCursor);

    if (image.format() != QImage::Format_RGBA8888) {
       _currentImage = image.convertToFormat(QImage::Format_RGBA8888);
    }
    else {
        _currentImage = image;
    }

    show();
}

void MainWindow::setupUi()
{
    // awesome font
    _awesome = new QtAwesome(this);
    _awesome->initFontAwesome();
    _awesome->setDefaultOption("scale-factor", 0.7);
    _awesome->setDefaultOption("color", QColor(100,50,70));
    _awesome->setDefaultOption("color-disabled", QColor(70,70,70,60));
    _awesome->setDefaultOption("color-active", QColor(180,60,80));
    _awesome->setDefaultOption("color-selected", QColor(200,70,90));

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
    _actionNew = new QAction(_awesome->icon(fa::cameraretro), tr("New"), this);
    // _actionOpen = new QAction(_awesome->icon(fa::filepictureo), tr("Open"), this);
    _actionCopy = new QAction(_awesome->icon(fa::copy), tr("Copy"), this);
    _actionSave = new QAction(_awesome->icon(fa::save), tr("Save"), this);

    connect(_actionNew, &_actionNew->triggered, this, &slotActionNew);
    // connect(_actionOpen, &_actionOpen->triggered, this, &slotActionOpen);
    connect(_actionCopy, &_actionCopy->triggered, this, &slotActionCopy);
    connect(_actionSave, &_actionSave->triggered, this, &slotActionSave);

    _toolbar->insertAction(0, _actionNew);
    // _toolbar->insertAction(0, _actionOpen);
    _toolbar->insertAction(0, _actionCopy);
    _toolbar->insertAction(0, _actionSave);

    // central widget
    _scrollArea = new QScrollArea(this);
    _scrollArea->setBackgroundRole(QPalette::BrightText);
    _scrollArea->setAlignment(Qt::AlignCenter);

    /*
    QListWidget* lw = new QListWidget(this);
    lw->setIconSize(QSize(64, 64));
    lw->setViewMode(QListWidget::IconMode);
    lw->setSelectionMode(QListView::ExtendedSelection);
    lw->setMovement(QListView::Static);
    new QListWidgetItem(awesome->icon(fa::aligncenter), tr("aligncenter"), lw);
    new QListWidgetItem(awesome->icon(fa::alignjustify), tr("alignjustify"), lw);
    new QListWidgetItem(awesome->icon(fa::alignleft), tr("alignleft"), lw);
    new QListWidgetItem(awesome->icon(fa::alignright), tr("alignright"), lw);
    */

    setCentralWidget(_scrollArea);

    // docked widgets
    _cropDock = new QDockWidget(tr("Crop"), this);
    _cropDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _cropDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    _cropDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, _cropDock);
    _cropDock->setWidget(new CropWidget(_cropDock));
    _cropDock->toggleViewAction()->setIcon(_awesome->icon(fa::crop));

    _colorsDock = new QDockWidget(tr("Colors"), this);
    _colorsDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _colorsDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    _colorsDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, _colorsDock);
    _colorsDock->toggleViewAction()->setIcon(_awesome->icon(fa::eyedropper));

    _highlightDock = new QDockWidget(tr("Highlight"), this);
    _highlightDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _highlightDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    _highlightDock->setVisible(false);
    addDockWidget(Qt::RightDockWidgetArea, _highlightDock);
    _highlightDock->toggleViewAction()->setIcon(_awesome->icon(fa::lightbulbo));

    _toolbar->insertSeparator(0);
    _toolbar->addAction(_cropDock->toggleViewAction());
    _toolbar->addAction(_colorsDock->toggleViewAction());
    _toolbar->addAction(_highlightDock->toggleViewAction());


    centerWindow();
}
