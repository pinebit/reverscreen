#include "clipboard.h"
#include "image.h"

#include <QApplication>
#include <QClipboard>

void copyToClipboard(const QImage& image){
    HBITMAP bitmap = image::convertQImageToHBITMAP(image);
    if(bitmap) {
        if (::OpenClipboard(NULL)){
            ::EmptyClipboard();
            ::SetClipboardData(CF_BITMAP, bitmap);
            ::CloseClipboard();
        }
        DeleteObject(bitmap);
    }
}

bool tryPasteFromClipboard(QImage& image) {
   image = QApplication::clipboard()->image();
   return (!image.isNull()) ? true : false;
}
