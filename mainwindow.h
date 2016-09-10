#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>

#include <memory>

namespace Ui {
    class MainWindow;
}

class QtAwesome;
class ImageCropWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionCopy_triggered();
    void on_actionSave_triggered();

private:
    QPixmap grabScreen();
    bool cropImage(std::shared_ptr<QPixmap> image);
    bool saveFile(const QString &fileName);
    void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
    void centerWindow();
    void delay(int millisecondsToWait);

    Ui::MainWindow *ui;
    QtAwesome *awesome;
    ImageCropWidget *imageCrop;
    QLabel* imageLabel;
};

#endif // MAINWINDOW_H
