#include "awesomeservice.h"
#include <QtAwesome.h>

AwesomeService* AwesomeService::s_instance = NULL;

void AwesomeService::create(QObject *parent)
{
    Q_ASSERT(s_instance == NULL);

    s_instance = new AwesomeService(parent);
}

QtAwesome *AwesomeService::awesome()
{
    Q_ASSERT(s_instance != NULL);

    return s_instance->_awesome;
}

AwesomeService::AwesomeService(QObject *parent) : QObject(parent)
{
    _awesome = new QtAwesome(this);
    _awesome->initFontAwesome();
}
