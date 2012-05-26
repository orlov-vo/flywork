/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef OPPCODES_H
#define OPPCODES_H
#include <QString>


enum Opcodes {
    MSG_NULL_ACTION                 = 0x000,
    FMSG_AUTH_START                 = 0x001,
    SMSG_AUTH_CHALLENGE             = 0x002,
    CMSG_AUTH_USERNAME              = 0x003,
    SMSG_AUTH_USERNAME_INVALID      = 0x004,
    SMSG_AUTH_USERNAME_USED         = 0x005,
    CMSG_AUTH_PASSWORD              = 0x006,
    SMSG_AUTH_PASSWORD_INVALID      = 0x007,
    SMSG_AUTH_SUCCESS               = 0x008,
    CMSG_WORK_INFO                  = 0x009,
    CMSG_WORK_STATUS                = 0x00A,
    SMSG_ACCESS_DENIED              = 0x00B,
    SMSG_WORK_INFO                  = 0x00C,
    SMSG_WORK_STATUS                = 0x00D,
    SMSG_WORK_INVALID               = 0x00E,
    CMSG_USER_INFO                  = 0x00F,
    SMSG_USER_INFO                  = 0x010,
    CMSG_ADMIN_INFO_SAVE            = 0x011,
    CMSG_ADMIN_WORK_PROCESS         = 0x012,
    SMSG_ADMIN_INFO_SAVE            = 0x013,
    SMSG_ADMIN_WORK_PROCESS         = 0x014,
    CMSG_ADMIN_USER_SAVE            = 0x015,
    SMSG_ADMIN_USER_SAVE            = 0x016,
    CMSG_ADMIN_GROUP_SAVE           = 0x017,
    SMSG_ADMIN_GROUP_SAVE           = 0x018,
    CMSG_ADMIN_USER_DEL             = 0x019,
    SMSG_ADMIN_USER_DEL             = 0x01A,
    CMSG_ADMIN_GROUP_DEL            = 0x01B,
    SMSG_ADMIN_GROUP_DEL            = 0x01C,
    CMSG_ADMIN_USERS_GET            = 0x01D,
    SMSG_ADMIN_USERS_GET            = 0x01E,
    CMSG_ADMIN_GROUPS_GET           = 0x01F,
    SMSG_ADMIN_GROUPS_GET           = 0x020,
    CMSG_ADMIN_TASK_SAVE            = 0x021,
    SMSG_ADMIN_TASK_SAVE            = 0x022,
    CMSG_ADMIN_TASK_DEL             = 0x023,
    SMSG_ADMIN_TASK_DEL             = 0x024,
    CMSG_ADMIN_TASKS_GET            = 0x025,
    SMSG_ADMIN_TASKS_GET            = 0x026,
    CMSG_TASK_SEND                  = 0x027,
    SMSG_TASK_SEND                  = 0x028,
    CMSG_SAVE_USERS_AND_GROUP       = 0x029,
    CMSG_SAVE_WORK                  = 0x02A,
    NUM_MSG_TYPES                   = 0x02B
};


enum SessionStatus {
    STATUS_AUTHED = 0,                                      // Player authenticated (_player == NULL, m_playerRecentlyLogout = false or will be reset before handler call, m_GUID have garbage)
    STATUS_LOGGEDIN,                                        // Player in game (_player != NULL, m_GUID == _player->GetGUID(), inWorld())
    STATUS_TRANSFER,                                        // Player transferring to another map (_player != NULL, m_GUID == _player->GetGUID(), !inWorld())
    STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT,                    // _player!= NULL or _player == NULL && m_playerRecentlyLogout, m_GUID store last _player guid)
    STATUS_NEVER,                                           // Opcode not accepted from client (deprecated or server side only)
    STATUS_UNHANDLED                                        // Opcode not handled yet
};

struct OpcodeHandler {
    QString name;
    SessionStatus status;
    //void (WorldSession::*handler)(WorldPacket& recvPacket);
};

extern OpcodeHandler OpcodeTable[NUM_MSG_TYPES];

/// Lookup opcode name for human understandable logging
extern inline QString LookupOpcodeName(quint16 id)
{
    if (id >= NUM_MSG_TYPES)
        return "Unknown opcode "+ QString::number(id, 16).toUpper() +", it's more than max!";
    return OpcodeTable[id].name + " - " + QString::number(id, 16).toUpper();
}

#endif // OPPCODES_H
