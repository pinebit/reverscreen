#pragma once

#include "assistant/snapassistant.h"

#include <QSharedPointer>


class CvModel;

class CvSnapAssistant : public SnapAssistant
{
public:
    CvSnapAssistant(QSharedPointer<CvModel> model);

    QRect snap(const QRect& rect);

private:
    QSharedPointer<CvModel> _model;
};
