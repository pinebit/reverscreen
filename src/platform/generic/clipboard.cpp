#include "clipboard.h"

#include <QApplication>
#include <QClipboard>

void copyToClipboard(const QImage& image){
    QApplication::clipboard()->setImage(image);
}

bool tryPasteFromClipboard(QImage& image) {
   image = QApplication::clipboard()->image();
   return (!image.isNull()) ? true : false;
}
