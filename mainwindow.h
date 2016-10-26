#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QScrollArea>
#include <QDockWidget>

class QtAwesome;
class RsView;
class AccentWidget;
class ColorsWidget;
class AccentPainter;
class CvModelBuilder;
class CvModel;
class RegionContext;

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
    void slotMouseMove(const QPoint& pos);

    void slotAccentChanged();
    void slotAccentApplied();

    void slotBuildCompleted(QSharedPointer<CvModel> model);

private:
    bool saveImage(const QString &fileName);
    bool openImage(const QString &fileName);
    void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
    void delay(int millisecondsToWait);
    void updateImage(const QImage& image);
    void updateImage(const QSharedPointer<RegionContext>& regionContext);

    void enableDisableUi();
    void handleDockWidgetVisibityChange(QDockWidget* dockWidget);
    QSharedPointer<AccentPainter> createDefaultAccentPainter();
    QSharedPointer<AccentPainter> createAccentPainter();
    void setupUi();
    void setupDockWidget(QDockWidget* dockWidget, QIcon icon, QWidget* contentWidget);

    QImage _currentImage;
    CvModelBuilder* _modelBuilder;

    QtAwesome* _awesome;
    RsView* _rsview;
    AccentWidget* _accentWidget;
    ColorsWidget* _colorsWidget;

    QScrollArea* _scrollArea;

    QToolBar* _toolbar;
    QStatusBar* _statusbar;

    QAction* _actionPaste;
    QAction* _actionOpen;
    QAction* _actionCopy;
    QAction* _actionSave;
    QAction* _actionCrop;

    QDockWidget* _colorsDock;
    QDockWidget* _accentDock;
};

