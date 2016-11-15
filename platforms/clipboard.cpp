#include <platforms/clipboard.h>
#include <platforms/image.h>

#include <QApplication>
#include <QClipboard>

#ifdef Q_OS_WIN
void imageToClipboard(const QImage& image){
    HBITMAP bitmap = convertQImageToHBITMAP(image);
    if(bitmap) {
        if (::OpenClipboard(NULL)){
            ::EmptyClipboard();
            ::SetClipboardData(CF_BITMAP, bitmap);
            ::CloseClipboard();
        }
        DeleteObject(bitmap);
    }
}
#else
void imageToClipboard(const QImage& image){
    QApplication::clipboard()->setImage(image);
}
#endif

QImage imageFromClipboard() {
    return QApplication::clipboard()->image();
}
