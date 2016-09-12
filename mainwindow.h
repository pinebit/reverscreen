#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QScrollArea>

class QtAwesome;
class RegionSelector;
class CvModelBuilder;
class CvModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void slotActionNew();
    void slotActionCopy();
    void slotActionSave();
    void slotActionOpen();
    void slotBuildCompleted(QSharedPointer<CvModel> model);

private:
    bool saveImage(const QString &fileName);
    bool openImage(const QString &fileName);
    void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
    void centerWindow();
    void delay(int millisecondsToWait);
    void updateImage(const QImage& image);
    void setupUi();

    // CV
    CvModelBuilder* _builder;
    QImage _currentImage;

    // UI
    QtAwesome* _awesome;
    RegionSelector* _regionSelector;
    QToolBar* _toolbar;
    QStatusBar* _statusbar;
    QAction* _actionNew;
    QAction* _actionOpen;
    QAction* _actionCopy;
    QAction* _actionSave;
    QScrollArea* _scrollArea;
};

