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

#include <QtAwesome.h>

#include <mainwindow.h>
#include <regionselector.h>
#include <fullscreenselectiondialog.h>
#include <fineselectionstrategy.h>
#include <snapselectionstrategy.h>
#include <CV/cvmodelbuilder.h>
#include <CV/cvmodel.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();

    _builder = new CvModelBuilder(this);
    connect(_builder, &_builder->signalBuildCompleted, this, &this->slotBuildCompleted);
}

void MainWindow::slotActionNew()
{
    hide();
    delay(300);

    QScreen *screen = QGuiApplication::primaryScreen();
    _currentImage = screen->grabWindow(QApplication::desktop()->winId()).toImage();

    FullscreenSelectionDialog dialog(this, _currentImage);
    if (dialog.exec() == QDialog::Accepted) {
        updateImage(dialog.getImage());
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

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void MainWindow::slotBuildCompleted(QSharedPointer<CvModel> model)
{
    QSharedPointer<SelectionStrategy> strategy(new SnapSelectionStrategy(model));
    _regionSelector = new RegionSelector(_scrollArea, _currentImage);
    _regionSelector->setSelectionStrategy(strategy, QCursor(Qt::CrossCursor));
    _scrollArea->setWidget(_regionSelector);

    setCursor(Qt::ArrowCursor);

    show();
}

bool MainWindow::saveFile(const QString &fileName)
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
    setCursor(Qt::WaitCursor);

    _currentImage = image;

    CvModelBuilderOptions options;
    _builder->buildAsync(_currentImage, options);
}

void MainWindow::setupUi()
{
    // awesome font
    _awesome = new QtAwesome(this);
    _awesome->initFontAwesome();
    _awesome->setDefaultOption("scale-factor", 0.8);
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

    // icon
    QIcon icon;
    icon.addFile(QStringLiteral(":/images/reverscreen.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);
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
    _actionNew = new QAction(_awesome->icon(fa::cameraretro), tr("NEW"), this);
    _actionCopy = new QAction(_awesome->icon(fa::copy), tr("COPY"), this);
    _actionSave = new QAction(_awesome->icon(fa::save), tr("SAVE"), this);

    connect(_actionNew, &_actionNew->triggered, this, &slotActionNew);
    connect(_actionCopy, &_actionCopy->triggered, this, &slotActionCopy);
    connect(_actionSave, &_actionSave->triggered, this, &slotActionSave);

    _toolbar->insertAction(0, _actionNew);
    _toolbar->insertAction(0, _actionCopy);
    _toolbar->insertAction(0, _actionSave);

    // central widget
    _scrollArea = new QScrollArea(this);
    _scrollArea->setBackgroundRole(QPalette::BrightText);

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

    centerWindow();
}
