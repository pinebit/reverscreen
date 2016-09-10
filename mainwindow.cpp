#include "mainwindow.h"
#include "ui_mainwindow.h"

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

#include <QtAwesome.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    awesome = new QtAwesome(this);
    awesome->initFontAwesome();
    awesome->setDefaultOption("scale-factor", 0.8);
    awesome->setDefaultOption("color", QColor(100,50,70));
    awesome->setDefaultOption("color-disabled", QColor(70,70,70,60));
    awesome->setDefaultOption("color-active", QColor(180,60,80));
    awesome->setDefaultOption("color-selected", QColor(200,70,90));

    ui->actionNew->setIcon(awesome->icon(fa::cameraretro));
    ui->actionCopy->setIcon(awesome->icon(fa::copy));
    ui->actionSave->setIcon(awesome->icon(fa::save));

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->setAlignment(Qt::AlignCenter);

    ui->scrollArea->setBackgroundRole(QPalette::BrightText);
    ui->scrollArea->setWidget(imageLabel);
    ui->scrollArea->setWidgetResizable(false);
    ui->scrollArea->setVisible(false);

    ui->actionCopy->setDisabled(true);
    ui->actionSave->setDisabled(true);

    setCentralWidget(ui->scrollArea);
    ui->statusBar->showMessage(tr("Click NEW button for a new snipping."));

    lastSelectionMode = ImageCropWidget::SnapSelection;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    this->hide();
    delay(300);

    std::shared_ptr<QPixmap> screenshot(new QPixmap(grabScreen()));

    if (cropImage(screenshot)) {
        imageLabel->setPixmap(*screenshot);
        imageLabel->adjustSize();
        ui->scrollArea->setVisible(true);

        resize(screenshot->width() + 40, screenshot->height() + 80);
        centerWindow();

        ui->actionCopy->setDisabled(false);
        ui->actionSave->setDisabled(false);
    }

    this->show();
}

void MainWindow::on_actionCopy_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(*imageLabel->pixmap());

    ui->statusBar->showMessage(tr("Copied to the clipboard."));
}

void MainWindow::on_actionSave_triggered()
{
    QFileDialog dialog(this, tr("Save Image As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void MainWindow::selectionModeChanged(ImageCropWidget::SelectionMode mode)
{
    lastSelectionMode = mode;
}

QPixmap MainWindow::grabScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    return screen->grabWindow(QApplication::desktop()->winId());
}

bool MainWindow::cropImage(std::shared_ptr<QPixmap> image)
{
    imageCrop = new ImageCropWidget(this);
    connect(imageCrop, &ImageCropWidget::selectionModeChanged, this, &selectionModeChanged);
    imageCrop->setImage(image, lastSelectionMode);

    if (!imageCrop->proceed()) {
        ui->statusBar->showMessage(tr("Snipping cancelled."));
        return false;
    }
    else
    {
        *image = imageCrop->getCroppedImage();
        delete imageCrop;
        ui->statusBar->showMessage(tr("Snipping created."));
        return true;
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
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
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
