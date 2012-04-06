/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "Opcodes.h"

OpcodeHandler OpcodeTable[NUM_MSG_TYPES] =
{
    /*0x000*/ { "MSG_NULL_ACTION",                  STATUS_NEVER },
    /*0x001*/ { "FMSG_AUTH_START",                  STATUS_NEVER },
    /*0x002*/ { "SMSG_AUTH_CHALLENGE",              STATUS_NEVER },
    /*0x003*/ { "CMSG_AUTH_USERNAME",               STATUS_NEVER },
    /*0x004*/ { "SMSG_AUTH_USERNAME_INVALID",       STATUS_NEVER },
    /*0x005*/ { "SMSG_AUTH_USERNAME_USED",          STATUS_NEVER },
    /*0x006*/ { "CMSG_AUTH_PASSWORD",               STATUS_NEVER },
    /*0x007*/ { "SMSG_AUTH_PASSWORD_INVALID",       STATUS_NEVER },
    /*0x008*/ { "SMSG_AUTH_SUCCESS",                STATUS_NEVER },
    /*0x009*/ { "CMSG_WORK_INFO",                   STATUS_NEVER },
    /*0x00A*/ { "CMSG_WORK_STATUS",                 STATUS_NEVER },
    /*0x00B*/ { "SMSG_ACCESS_DENIED",               STATUS_NEVER },
    /*0x00C*/ { "SMSG_WORK_INFO",                   STATUS_NEVER },
    /*0x00D*/ { "SMSG_WORK_STATUS",                 STATUS_NEVER },
    /*0x00E*/ { "SMSG_WORK_INVALID",                STATUS_NEVER },
    /*0x00F*/ { "CMSG_USER_INFO",                   STATUS_NEVER },
    /*0x010*/ { "SMSG_USER_INFO",                   STATUS_NEVER },
    /*0x011*/ { "CMSG_ADMIN_INFO_SAVE",             STATUS_NEVER },
    /*0x012*/ { "CMSG_ADMIN_WORK_PROCESS",          STATUS_NEVER },
    /*0x013*/ { "SMSG_ADMIN_INFO_SAVE",             STATUS_NEVER },
    /*0x014*/ { "SMSG_ADMIN_WORK_PROCESS",          STATUS_NEVER },
    /*0x015*/ { "CMSG_ADMIN_USER_SAVE",             STATUS_NEVER },
    /*0x016*/ { "SMSG_ADMIN_USER_SAVE",             STATUS_NEVER },
    /*0x017*/ { "CMSG_ADMIN_GROUP_SAVE",            STATUS_NEVER },
    /*0x018*/ { "SMSG_ADMIN_GROUP_SAVE",            STATUS_NEVER },
    /*0x019*/ { "CMSG_ADMIN_USER_DEL",              STATUS_NEVER },
    /*0x01A*/ { "SMSG_ADMIN_USER_DEL",              STATUS_NEVER },
    /*0x01B*/ { "CMSG_ADMIN_GROUP_DEL",             STATUS_NEVER },
    /*0x01C*/ { "SMSG_ADMIN_GROUP_DEL",             STATUS_NEVER },
    /*0x01D*/ { "CMSG_ADMIN_USERS_GET",             STATUS_NEVER },
    /*0x01E*/ { "SMSG_ADMIN_USERS_GET",             STATUS_NEVER },
    /*0x01F*/ { "CMSG_ADMIN_GROUPS_GET",            STATUS_NEVER },
    /*0x020*/ { "SMSG_ADMIN_GROUPS_GET",            STATUS_NEVER },
    /*0x021*/ { "CMSG_ADMIN_TASK_SAVE",             STATUS_NEVER },
    /*0x022*/ { "SMSG_ADMIN_TASK_SAVE",             STATUS_NEVER },
    /*0x023*/ { "CMSG_ADMIN_TASK_DEL",              STATUS_NEVER },
    /*0x024*/ { "SMSG_ADMIN_TASK_DEL",              STATUS_NEVER },
    /*0x025*/ { "CMSG_ADMIN_TASKS_GET",             STATUS_NEVER },
    /*0x026*/ { "SMSG_ADMIN_TASKS_GET",             STATUS_NEVER },
    /*0x027*/ { "CMSG_TASK_SEND",                   STATUS_NEVER },
    /*0x028*/ { "SMSG_TASK_SEND",                   STATUS_NEVER },
    /*0x029*/ { "CMSG_SAVE_USERS_AND_GROUP",        STATUS_NEVER }
};
