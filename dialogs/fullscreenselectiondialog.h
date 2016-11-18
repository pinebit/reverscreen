#pragma once

#include <QDialog>
#include <QImage>
#include <QSharedPointer>

class RsView;
class CvModelBuilder;
class CvModel;
class AccentPainter;

class FullscreenSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    FullscreenSelectionDialog(QWidget* parent, const QImage& image, QSharedPointer<AccentPainter> accentPainter);

    QRect getSelectedRect() const;

private slots:
    void slotBuildCompleted(QSharedPointer<CvModel> model);
    void slotSelectionFinished();

private:
    RsView* _rsview;
    CvModelBuilder* _builder;
    QSharedPointer<AccentPainter> _accentPainter;
};

