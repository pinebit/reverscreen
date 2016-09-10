// Qt
#include <QPainter>

// app
#include <regionselector.h>

RegionSelector::RegionSelector(QWidget *parent, const QImage& image, QSharedPointer<CvModel> model)
    : QWidget(parent)
    , _image(image)
    , _model(model)
{
}

void RegionSelector::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);

    painter.drawImage(0, 0, _image);
}
