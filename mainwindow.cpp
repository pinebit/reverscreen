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

#include <QtAwesome.h>

#include <mainwindow.h>
#include <regionselector.h>
#include <fineselectionstrategy.h>
#include <snapselectionstrategy.h>
#include <CV/cvmodelbuilder.h>
#include <CV/cvmodel.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();

    builder = new CvModelBuilder(this);
    connect(builder, &builder->signalBuildCompleted, this, &this->slotBuildCompleted);

    grabScreenshot();
}

void MainWindow::slotActionNew()
{
    hide();
    delay(300);

    grabScreenshot();
}

void MainWindow::slotActionCopy()
{
    /*
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(*imageLabel->pixmap());

    ui->statusBar->showMessage(tr("Copied to the clipboard."));
    */
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
    regionSelector = new RegionSelector(scrollArea, currentImage);
    regionSelector->setSelectionStrategy(strategy, QCursor(Qt::CrossCursor));
    scrollArea->setWidget(regionSelector);

    setCursor(Qt::ArrowCursor);

    show();
}

bool MainWindow::saveFile(const QString &fileName)
{
    /*
    QImageWriter writer(fileName);

    const QImage& image = imageLabel->pixmap()->toImage();
    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot save %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Saved \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    */

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
    int w = qMin(width(), qApp->desktop()->width() - 200);
    int h = qMin(height(), qApp->desktop()->height() - 200);
    resize(w, h);

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

void MainWindow::grabScreenshot()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    currentImage = screen->grabWindow(QApplication::desktop()->winId()).toImage();

    setCursor(Qt::WaitCursor);

    CvModelBuilderOptions options;
    builder->buildAsync(currentImage, options);
}

void MainWindow::setupUi()
{
    // awesome font
    awesome = new QtAwesome(this);
    awesome->initFontAwesome();
    awesome->setDefaultOption("scale-factor", 0.8);
    awesome->setDefaultOption("color", QColor(100,50,70));
    awesome->setDefaultOption("color-disabled", QColor(70,70,70,60));
    awesome->setDefaultOption("color-active", QColor(180,60,80));
    awesome->setDefaultOption("color-selected", QColor(200,70,90));

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
    toolbar = new QToolBar(this);
    toolbar->setFont(font);
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(Qt::TopToolBarArea, toolbar);

    // statusbar
    statusbar = new QStatusBar(this);
    statusbar->setFont(font);
    setStatusBar(statusbar);
    statusbar->showMessage(tr("Ready."));

    // actions
    actionNew = new QAction(awesome->icon(fa::cameraretro), tr("NEW"), this);
    actionCopy = new QAction(awesome->icon(fa::copy), tr("COPY"), this);
    actionSave = new QAction(awesome->icon(fa::save), tr("SAVE"), this);

    connect(actionNew, &actionNew->triggered, this, &slotActionNew);
    connect(actionCopy, &actionCopy->triggered, this, &slotActionCopy);
    connect(actionSave, &actionSave->triggered, this, &slotActionSave);

    toolbar->insertAction(0, actionNew);
    toolbar->insertAction(0, actionCopy);
    toolbar->insertAction(0, actionSave);

    // central widget
    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::BrightText);
    scrollArea->setWidget(new QLabel(tr("LOADING...")));

    setCentralWidget(scrollArea);
}
