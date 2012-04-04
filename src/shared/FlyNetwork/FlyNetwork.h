/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYNETWORK_H
#define FLYNETWORK_H

#include <QObject>
#include <QTcpSocket>

class FlyNetwork
{
public:
    static void doSendPacket(QTcpSocket *_sok, quint16 _command, QByteArray _block);
    static void doSendPacket(QTcpSocket *_sok, quint16 _command);
    static void debugCommand(quint16 command, quint16 packetsize);
};

#endif // FLYNETWORK_H
