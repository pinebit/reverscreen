#pragma once

#include <QObject>
#include <QtAwesome.h>

class QtAwesome;

class AwesomeService : public QObject
{
public:
    static void create(QObject *parent);
    static QtAwesome* awesome();

private:
    AwesomeService(QObject *parent);

    static AwesomeService* s_instance;
    QtAwesome* _awesome;
};
