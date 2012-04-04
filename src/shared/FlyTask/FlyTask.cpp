/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyTask.h"

FlyTask::FlyTask(QObject *parent) :
    QObject(parent)
{
    setId(0);
    setType(fTask::inputBox);
    setName("NoName");
    setContent("Description");
    setAnswer("Answer");
}
