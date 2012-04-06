/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyWork.h"

FlyWork::FlyWork(QObject *parent) :
    QObject(parent)
{
    setId("untitled");
    setTitle(tr("Работа без названия"));
    setEndTime(QTime::currentTime().addSecs(2400));
    setStatus(fWork::statusPause);
    setValid(false);
}
