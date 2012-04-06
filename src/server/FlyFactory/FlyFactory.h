/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYFACTORY_H
#define FLYFACTORY_H

#include <QDebug>

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QCryptographicHash>

#include "FlyAnswer.h"
#include "FlyUser.h"
#include "FlyTask.h"
#include "FlyWork.h"
#include "FlyGroup.h"

class FlyFactory
{
public:
    static QList<FlyUser *> users();
    static QList<FlyAnswer *> answers(FlyWork *work, FlyUser *user);
    static QList<FlyTask *> tasks();
    static QList<FlyGroup *> groups();
    static FlyWork* work();

    static void saveWork(FlyWork *work, QList<FlyTask *> tasks);
    static void saveUsersAndGroups(QList<FlyUser *> users, QList<FlyGroup *> groups);
    static void saveTasks(FlyWork *work, QList<FlyTask *> tasks, FlyUser *user, QList<FlyAnswer *> answers);
};

#endif // FLYFACTORY_H
