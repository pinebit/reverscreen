#pragma once

#include <QImage>

void copyToClipboard(const QImage& image);
bool tryPasteFromClipboard(QImage& image);
