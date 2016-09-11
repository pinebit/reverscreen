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
    void slotBuildCompleted(QSharedPointer<CvModel> model);

private:
    bool saveFile(const QString &fileName);
    void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);
    void centerWindow();
    void delay(int millisecondsToWait);
    void grabScreenshot();
    void setupUi();

    // CV
    CvModelBuilder* builder;
    QImage currentImage;

    // UI
    QtAwesome *awesome;
    RegionSelector *regionSelector;
    QToolBar* toolbar;
    QStatusBar* statusbar;
    QAction* actionNew;
    QAction* actionCopy;
    QAction* actionSave;
    QScrollArea* scrollArea;
};

