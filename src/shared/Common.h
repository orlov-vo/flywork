/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef COMMON_H
#define COMMON_H

namespace fTask
{
    enum typeTask {
        inputBox    = 0x001,
        radioBox    = 0x002,
        checkBox    = 0x003,
        tempFile    = 0x004
    };
}

namespace fWork
{
    enum WorkStatus {
        statusNone,
        statusStart,
        statusStop,
        statusPause
    };
}

#endif // COMMON_H
