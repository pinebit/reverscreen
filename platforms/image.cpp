
#include <platforms/image.h>

#ifdef Q_OS_WIN

HBITMAP convertQImageToHBITMAP(const QImage& image){
    if (image.isNull())
       return 0;

    HDC displayDC = ::GetDC(NULL);
    HDC memoryDC = ::CreateCompatibleDC(NULL);
    if (!displayDC || !memoryDC){
        return 0;
    }
    // it is a memory memoryDC that must have a handle selected into it
    HBITMAP dummyBitmap = ::CreateCompatibleBitmap(displayDC, image.width(), image.height());
    ::ReleaseDC(NULL, displayDC);
    HBITMAP oldBitmap = (HBITMAP) ::SelectObject(memoryDC, dummyBitmap);

    BITMAPINFO info;
    ::memset(&info, 0, sizeof(info));
    info.bmiHeader.biBitCount = image.depth();
    info.bmiHeader.biClrImportant = 0;
    info.bmiHeader.biClrUsed = 0;
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biHeight = -image.height();
    info.bmiHeader.biWidth = image.width();
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biSizeImage = image.width() * image.height() * 4;

    HBITMAP bitmap = ::CreateDIBitmap(memoryDC, &info.bmiHeader, CBM_INIT, image.constBits(),(LPBITMAPINFO) &info, DIB_RGB_COLORS);
    if (bitmap) {
        ::SelectObject(memoryDC, oldBitmap);
        ::DeleteObject(dummyBitmap);
        ::DeleteDC(memoryDC);
    }
    return bitmap;
}

HBITMAP captureQWidget(const QWidget& widget){
    HWND hWnd = (HWND)widget.winId();
    HDC displayDC = ::GetDC(hWnd);
    if (!displayDC){
        return 0;
    }
    RECT rectWindow;
    ::GetWindowRect(hWnd, &rectWindow);

    HDC memoryDC = ::CreateCompatibleDC(displayDC);
    HBITMAP bitmap = ::CreateCompatibleBitmap(displayDC, rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top);
    if (bitmap) {
        ::SelectObject(memoryDC, bitmap);
        ::BitBlt(memoryDC, 0, 0, rectWindow.right - rectWindow.left, rectWindow.bottom - rectWindow.top, displayDC, rectWindow.left, rectWindow.top, SRCCOPY);
    }
    ::ReleaseDC(hWnd, memoryDC);
    return bitmap;
}

#endif
