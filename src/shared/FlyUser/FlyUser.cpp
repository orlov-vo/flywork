/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyUser.h"

FlyUser::FlyUser(QObject *parent) :
    QObject(parent)
{
    setGuid(0);
    setLogin("");
    setUsername("");
    setHash("");
    setGroupId(0);
    setAdmin(false);
}
