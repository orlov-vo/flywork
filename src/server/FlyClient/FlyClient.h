#ifndef FLYCLIENT_H
#define FLYCLIENT_H
/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QThreadPool>
#include <QRegExp>

#include "FlyServer.h"
#include "FlyUser.h"

#include "FlyNetwork.h"
#include "Opcodes.h"

class FlyServer;
class FlyNetwork;

class FlyClient : public QObject
{
    friend class FlyServer;
    Q_OBJECT

public:
    explicit FlyClient(int desc, FlyServer *serv, QObject *parent = 0);
    ~FlyClient();

    FlyUser *getUser() { return _user; }

    void doSendPacket(quint16 command, QByteArray block) {
        FlyNetwork::doSendPacket(_sok, command, block);
    }
    void doSendPacket(quint16 command) {
        FlyNetwork::doSendPacket(_sok, command);
    }

signals:
    void removeUser(FlyClient *client);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError) const;

private:
    QTcpSocket *_sok;
    FlyServer *_serv;
    quint16 _blockSize;
    bool _isAuthed;
    bool _state;
    FlyUser *_user;

    QString t_username;
    QByteArray t_hash;


};

#endif // MYCLIENT_H
