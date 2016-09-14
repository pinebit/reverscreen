#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QScrollArea>
#include <QDockWidget>

class QtAwesome;
class RegionSelector;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void slotActionScreenshot();
    void slotActionPaste();
    void slotActionOpen();

    void slotActionCopy();
    void slotActionSave();

    void slotActionCrop();
    void slotSelectionStarted();
    void slotRemoveColor(QColor color);

signals:
    void signalColorPicked(QColor color);

private:
    bool saveImage(const QString &fileName);
    bool openImage(const QString &fileName);
    void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
    void centerWindow();
    void delay(int millisecondsToWait);
    void updateImage(const QImage& image);
    void setupUi();
    void enableDisableUi();

    QImage _currentImage;

    // UI
    QtAwesome* _awesome;
    RegionSelector* _regionSelector;
    QScrollArea* _scrollArea;

    QToolBar* _toolbar;
    QStatusBar* _statusbar;

    QAction* _actionPaste;
    QAction* _actionOpen;
    QAction* _actionCopy;
    QAction* _actionSave;
    QAction* _actionCrop;

    QDockWidget* _colorsDock;
    QDockWidget* _cropDock;
    QDockWidget* _accentDock;
};

