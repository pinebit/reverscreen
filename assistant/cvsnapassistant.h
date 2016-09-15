#pragma once

#include <assistant/snapassistant.h>

#include <QSharedPointer>


class CvModel;

class CvSnapAssistant : public SnapAssistant
{
public:
    CvSnapAssistant(QSharedPointer<CvModel> model);

    QPoint begin(const QPoint &point);
    QPoint end(const QRect &rect);

private:
    QSharedPointer<CvModel> _model;
};
