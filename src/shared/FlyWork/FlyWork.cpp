/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyWork.h"

FlyWork::FlyWork(QObject *parent) :
    QObject(parent)
{
    setTitle(tr("Работа без названия"));
    setEndTime(QTime::currentTime().addSecs(2400));
    setStatus(fWork::statusPause);
    setValid(false);
}
