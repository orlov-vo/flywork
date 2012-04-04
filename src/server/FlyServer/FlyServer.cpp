/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyServer.h"
#include <QDebug>

FlyServer::FlyServer(QObject *parent) :QTcpServer(parent)
{
    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(doUpdateTime()));
    //connect(this, SIGNAL(updateWorkStatus(fWork::WorkStatus)), this, SLOT(onUpdateWorkStatus(fWork::WorkStatus)));
}

bool FlyServer::doStartServer(QHostAddress addr, qint16 port)
{
    _users = FlyFactory::users();
    _groups = FlyFactory::groups();
    _work = FlyFactory::work();
    _tasks = FlyFactory::tasks();


    if (!listen(addr, port)) {
        qDebug() << "Server not started at" << addr.toString() << ":" << port;
        return false;
    }

    _timer->start(1000);

    qDebug() << "Server started at" << addr.toString() << ":" << port;
    return true;
}

bool FlyServer::isNameValid(QString name) const
{
    if (name.length() > 20 || name.length() < 5)
        return false;
    QRegExp req("[A-Za-z0-9_]+");
    return req.exactMatch(name);
}

bool FlyServer::isNameUsed(QString name) const
{
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->_isAuthed == true)
            if (_clients.at(i)->getUser()->login() == name)
                return true;
    return false;
}

FlyUser* FlyServer::searchUser(quint32 guid)
{
    for (int i = 0; i < _users.size(); ++i) {
        if (_users.at(i)->guid() == guid)
            return _users.at(i);
    }
    return NULL;
}

FlyUser* FlyServer::searchUser(QString name)
{
    for (int i = 0; i < _users.size(); ++i) {
        if (_users.at(i)->login() == name)
            return _users.at(i);
    }
    return NULL;
}

FlyGroup* FlyServer::searchGroup(quint32 id)
{
    for (int i = 0; i < _groups.size(); ++i) {
        if (_groups.at(i)->id() == id)
            return _groups.at(i);
    }
    return NULL;
}

FlyTask* FlyServer::searchTask(quint32 id)
{
    for (int i = 0; i < _tasks.size(); ++i) {
        if (_tasks.at(i)->id() == id)
            return _tasks.at(i);
    }
    return NULL;
}

bool FlyServer::isPasswordTrue(FlyUser *user, QByteArray hash)
{
    if (user->hash() == hash)
        return true;
    return false;
}

void FlyServer::incomingConnection(int handle)
{
    //создаем клиента
    FlyClient *client = new FlyClient(handle, this, this);
    connect(client, SIGNAL(removeUser(FlyClient*)), this, SLOT(onRemoveUser(FlyClient*)));
    _clients.append(client);
}

void FlyServer::onRemoveUser(FlyClient *client)
{
    _clients.removeAt(_clients.indexOf(client));
}

void FlyServer::doSendToAllPacket(quint16 command)
{
    for (int i = 0; i < _clients.size(); i++)
        FlyNetwork::doSendPacket(_clients.at(i)->_sok, command);
}

void FlyServer::doSendToAllPacket(quint16 command, QByteArray block)
{
    for (int i = 0; i < _clients.size(); i++)
        FlyNetwork::doSendPacket(_clients.at(i)->_sok, command, block);
}

void FlyServer::doSendWorkInfo()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << _work->title();
    out << _work->endTime();
    out << _tasks.size();
    for (int i = 0; i < _tasks.size(); i++) {
        out << (quint32)_tasks.at(i)->id();
        out << (quint8)_tasks.at(i)->type();
        out << _tasks.at(i)->name();
        out << _tasks.at(i)->content();
        out << _tasks.at(i)->answer();
    }
    doSendToAllPacket(SMSG_WORK_INFO, block);
    _work->setValid(true);
}

void FlyServer::doSendWorkInfo(FlyClient *client)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << _work->title();
    out << _work->endTime();
    out << _tasks.size();
    for (int i = 0; i < _tasks.size(); i++) {
        out << (quint32)_tasks.at(i)->id();
        out << (quint8)_tasks.at(i)->type();
        out << _tasks.at(i)->name();
        out << _tasks.at(i)->content();
        out << _tasks.at(i)->answer();
    }
    FlyNetwork::doSendPacket(client->_sok, SMSG_WORK_INFO, block);
}

void FlyServer::doUpdateTime()
{
    if (QTime::currentTime().secsTo(_work->endTime()) == 0)
        setWorkStatus(fWork::statusStop);
}

void FlyServer::setWork(QString title, QTime endTime)
{
    _work->setTitle(title);
    _work->setEndTime(endTime);
    emit updateWork();
}

void FlyServer::setTasks(QList<FlyTask *> tasks)
{
    _tasks = tasks;
    emit updateTasks();
}

void FlyServer::setWorkStatus(fWork::WorkStatus status)
{
    if (status == _work->status())
        return;

    _work->setStatus(status);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint8)status;
    doSendToAllPacket(SMSG_WORK_STATUS, block);

    emit updateWorkStatus(status);
//}

//void FlyServer::onUpdateWorkStatus(fWork::WorkStatus status)
//{
    switch(status) {
    case fWork::statusStart:
        this->doSendWorkInfo();
        break;
    case fWork::statusPause:
        break;
    case fWork::statusStop:
        FlyFactory::saveWork(_work, _tasks);
        break;
    }
}

