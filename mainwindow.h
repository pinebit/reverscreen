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
class ColorsWidget;
class MarkerWidget;
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
    void slotActionCapture();
    void slotActionPaste();
    void slotActionOpen();
    void slotActionCopy();
    void slotActionSave();
    void slotActionCrop();

    void slotSelectionChanged();
    void slotMouseMove(const QPoint& pos);

    void slotMarkerUndo();
    void slotMarkerChanged();

    void slotBuildCompleted(QSharedPointer<CvModel> model);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    enum State {
        EmptyState,
        CropState,
        ColorState,
        MarkerState
    };

    bool saveImage(const QString &fileName);
    bool openImage(const QString &fileName);
    void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
    void delay(int millisecondsToWait);
    void updateImage(const QImage& image);
    void updateImage(const QSharedPointer<RegionContext>& regionContext);

    void handleDockWidgetVisibityChange(QDockWidget* dockWidget);
    QSharedPointer<AccentPainter> createDefaultAccentPainter();
    QSharedPointer<AccentPainter> createMarkerAccentPainter();
    void setupUi();
    void setupDockWidget(QDockWidget* dockWidget, QIcon icon, QWidget* contentWidget);

    void changeState(State state);

    QImage _currentImage;
    CvModelBuilder* _modelBuilder;
    QSharedPointer<CvModel> _model;
    State _state;

    QtAwesome* _awesome;
    RsView* _rsview;
    MarkerWidget* _markerWidget;
    ColorsWidget* _colorsWidget;
    QDockWidget* _markerDock;
    QDockWidget* _colorsDock;

    QScrollArea* _scrollArea;

    QToolBar* _toolbar;
    QStatusBar* _statusbar;

    QAction* _actionCapture;
    QAction* _actionPaste;
    QAction* _actionCopy;
    QAction* _actionSave;
    QAction* _actionCrop;
};

