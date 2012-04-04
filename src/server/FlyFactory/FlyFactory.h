/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYFACTORY_H
#define FLYFACTORY_H

#include <QDebug>

#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>

#include "FlyUser.h"
#include "FlyTask.h"
#include "FlyWork.h"
#include "FlyGroup.h"

class FlyFactory
{
public:
    static QList<FlyUser *> users();
    static QList<FlyTask *> tasks();
    static QList<FlyGroup *> groups();
    static FlyWork* work();

    static void saveWork(FlyWork* work, QList<FlyTask *> tasks);
};

#endif // FLYFACTORY_H
