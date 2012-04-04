/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyNetwork.h"
#include "Opcodes.h"
#include <QDebug>

void FlyNetwork::doSendPacket(QTcpSocket *sok, quint16 command, QByteArray block)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out << (quint16)0;
    out << command;
    packet.append(block);
    out.device()->seek(0);
    out << (quint16)(packet.size() - (sizeof(quint16) + sizeof(quint16)));
    sok->write(packet);
    sok->waitForBytesWritten();
    sok->flush();
    qDebug() << "Send packet:\t\t" << LookupOpcodeName(command) << "(" + QString::number(packet.size() - sizeof(quint16)) + ")";
}

void FlyNetwork::doSendPacket(QTcpSocket *sok, quint16 _command)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << _command;
    sok->write(block);
    sok->waitForBytesWritten();
    sok->flush();
    qDebug() << "Send command:\t\t" << LookupOpcodeName(_command);
}

void FlyNetwork::debugCommand(quint16 command, quint16 packetsize)
{
    if (packetsize == 0)
        qDebug() << "Received command:\t" << LookupOpcodeName(command);
    else
        qDebug() << "Received command:\t" << LookupOpcodeName(command) << "(" + QString::number((int)packetsize) + ")";
}
