/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYUSER_H
#define FLYUSER_H

#include <QObject>

#include "Common.h"

class FlyUser : public QObject
{
    Q_OBJECT
public:
    explicit FlyUser(QObject *parent = 0);

    void setGuid(quint32 guid) { _userGUID = guid; }
    quint32 guid() { return _userGUID; }
    void setLogin(QString login) { _userLogin = login; }
    QString login() { return _userLogin; }
    void setUsername(QString name) { _userName = name; }
    QString username() { return _userName; }
    void setHash(QByteArray hash) { _userHash = hash; }
    QByteArray hash() { return _userHash; }
    void setGroupId(quint32 groupId) { _userGroupId = groupId; }
    quint32 groupId() { return _userGroupId; }
    void setGroupName(QString groupName) { _userGroupName = groupName; }
    QString groupName() { return _userGroupName; }
    void setAdmin(bool isAdmin) { _isAdmin = isAdmin; }
    bool isAdmin() { return _isAdmin; }
    
private:
    quint32 _userGUID;
    QString _userLogin;
    QString _userName;
    QByteArray _userHash;
    quint32 _userGroupId;
    QString _userGroupName;
    bool _isAdmin;
    
};

#endif // FLYUSER_H
