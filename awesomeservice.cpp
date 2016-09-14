#include <awesomeservice.h>
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
    _awesome->setDefaultOption("scale-factor", 0.7);
    _awesome->setDefaultOption("color", QColor(100,50,70));
    _awesome->setDefaultOption("color-disabled", QColor(70,70,70,60));
    _awesome->setDefaultOption("color-active", QColor(180,60,80));
    _awesome->setDefaultOption("color-selected", QColor(200,70,90));
}
