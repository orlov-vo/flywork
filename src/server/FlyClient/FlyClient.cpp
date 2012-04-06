/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyClient.h"

#include <QStringList>

FlyClient::FlyClient(int desc, FlyServer *serv, QObject *parent) :QObject(parent)
{
    // Сохраняем указатель на объект-сервер
    _serv = serv;

    // Размер принимаемого блока 0
    _blockSize = 0;

    _isAuthed = false;
    _state = false;

    // Создаем сокет
    _sok = new QTcpSocket(this);

    // Устанавливаем дескриптор из incomingConnection()
    _sok->setSocketDescriptor(desc);

    // Подключаем сигналы
    connect(_sok, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(_sok, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(_sok, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_sok, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));

    qDebug() << "Client connected" << desc;
}

FlyClient::~FlyClient()
{
}

void FlyClient::onConnect()
{
    //never calls, socket already connected to the tcpserver
    //we just binding to this socket here: _sok->setSocketDescriptor(desc);
}

void FlyClient::onDisconnect()
{
    qDebug() << "Client disconnected";
    FlyFactory::saveTasks(_serv->work(), _serv->tasks(), _user, _answers);
    // Убираем из списка
    emit removeUser(this);
}

void FlyClient::onError(QAbstractSocket::SocketError socketError) const
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Error", "The host was not found";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Error", "The connection was refused by the peer.";
        break;
    default:
        qDebug() << "Error", "The following error occurred: "+_sok->errorString();
    }
}

void FlyClient::onReadyRead()
{
    QDataStream in(_sok);
    quint16 command;
    if (_blockSize == 0) {
        if (_sok->bytesAvailable() < (int)(sizeof(quint16) + sizeof(quint16)))
            return;
        in >> _blockSize;
        in >> command;
        FlyNetwork::debugCommand(command, _blockSize);
    }

    if (_sok->bytesAvailable() < _blockSize)
        return;
    else
        _blockSize = 0;

    if (!_isAuthed) {
        if(_state) {
            if (command != CMSG_AUTH_PASSWORD)
                return;
        } else {
            if (command != CMSG_AUTH_USERNAME)
                return;
        }
    }

    switch(command) {
    case CMSG_AUTH_USERNAME:
    {
        // [1] Cчитываем имя
        QString name;
        in >> name;

        // [2] Проверяем его имя на валидность
        if (!_serv->isNameValid(name))
        {
            FlyNetwork::doSendPacket(_sok, SMSG_AUTH_USERNAME_INVALID);
            _sok->disconnectFromHost();
            return;
        }

        // [3] Проверяем не используется ли имя
        if (_serv->isNameUsed(name))
        {
            FlyNetwork::doSendPacket(_sok, SMSG_AUTH_USERNAME_USED);
            _sok->disconnectFromHost();
            return;
        }

        // [4] Ищем пользователя
        FlyUser *user = _serv->searchUser(name);

        // [5] Проверяем на существование имени
        if (user == NULL)
        {
            FlyNetwork::doSendPacket(_sok, SMSG_AUTH_USERNAME_INVALID);
            _sok->disconnectFromHost();
            return;
        }

        // [6] Запрашиваем пароль для сравнения
        FlyNetwork::doSendPacket(_sok, SMSG_AUTH_CHALLENGE);

        // [7] Сохраняем пользователя для дальнейшего использования
        _user = user;
        _state = true;
    }
        break;
    case CMSG_AUTH_PASSWORD:
    {
        // [1] Считываем Hash пароля
        QByteArray data;
        in >> data;

        // [2] Проверяем Hash в базе данных
        if (!_serv->isPasswordTrue(_user, data))
        {
            FlyNetwork::doSendPacket(_sok, SMSG_AUTH_PASSWORD_INVALID);
            _sok->disconnectFromHost();
            return;
        }

        // [3] Авторизация пройдена
        _isAuthed = true;
        FlyNetwork::doSendPacket(_sok, SMSG_AUTH_SUCCESS);
    }
        break;
    case CMSG_USER_INFO:
    {
        FlyGroup *group = _serv->searchGroup(_user->groupId());
        if (group == NULL)
            group = new FlyGroup();
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << _user->guid();
        out << _user->username();
        out << _user->groupId();
        out << group->name();
        out << _user->isAdmin();
        FlyNetwork::doSendPacket(_sok, SMSG_USER_INFO, block);
    }
        break;
    case CMSG_WORK_INFO:
    {
        if(_serv->work()->isValid())
        {
            _serv->doSendWorkInfo(this);
            _answers = FlyFactory::answers(_serv->work(), _user);
            doSendAnswers();
        }
    }
        break;
    case CMSG_WORK_STATUS:
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint8)_serv->work()->status();
        FlyNetwork::doSendPacket(_sok, SMSG_WORK_STATUS, block);
    }
        break;
    case CMSG_ADMIN_INFO_SAVE:
    {
        QString title; QTime endTime;
        in >> title; _serv->work()->setTitle(title);
        in >> endTime; _serv->work()->setEndTime(endTime);
        _serv->work()->setValid(true);
        _serv->doSendWorkInfo();
    }
        break;
    case CMSG_ADMIN_WORK_PROCESS:
    {
        quint8 status;
        in >> status; _serv->setWorkStatus(static_cast<fWork::WorkStatus>(status));
    }
        break;
    case CMSG_ADMIN_USER_SAVE:
    {
        quint32 guid; QString login; QString name; QByteArray hash; quint32 groupId; bool isAdmin; bool ishash;
        bool isNewUser = false;

        in >> guid;
        FlyUser *user = _serv->searchUser(guid);
        if (user == NULL)
        {
            user = new FlyUser();
            user->setGuid(guid);
            isNewUser = true;
        }

        in >> login; user->setLogin(login);
        in >> name; user->setUsername(name);
        in >> ishash;
        if(ishash)
        {
            in >> hash; user->setHash(hash);
        }
        else
        {
            if (isNewUser)
            {
                QByteArray str;
                str.append(user->username()+":password:flywR");
                user->setHash(QCryptographicHash::hash(str, QCryptographicHash::Sha1));
            }
        }

        in >> groupId; user->setGroupId(groupId);
        in >> isAdmin; user->setAdmin(isAdmin);

        if (isNewUser)
            _serv->addUser(user);
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_USER_SAVE);
    }
        break;
    case CMSG_ADMIN_GROUP_SAVE:
    {
        quint32 id; QString name;
        bool isNewGroup = false;
        in >> id;
        FlyGroup * group = _serv->searchGroup(id);
        if (group == NULL)
        {
            group = new FlyGroup();
            group->setId(id);
            isNewGroup = true;
        }
        in >> name; group->setName(name);

        if (isNewGroup)
            _serv->addGroup(group);
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_GROUP_SAVE);
    }
        break;
    case CMSG_ADMIN_TASK_SAVE:
    {
        quint32 id; quint8 type; QString name; QString desc; QString ans;

        bool isNewTask = false;
        in >> id;
        FlyTask * task = _serv->searchTask(id);
        if (task == NULL)
        {
            task = new FlyTask();
            task->setId(id);
            isNewTask = true;
        }
        in >> type; task->setType(static_cast<fTask::typeTask>(type));
        in >> name; task->setName(name);
        in >> desc; task->setContent(desc);
        in >> ans; task->setAnswer(ans);

        if (isNewTask)
            _serv->addTask(task);
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_TASK_SAVE);
    }
        break;
    case CMSG_ADMIN_USERS_GET:
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (int)_serv->users().size();
        for (int i = 0; i < _serv->users().size(); i++)
        {
            out << _serv->users().at(i)->guid();
            out << _serv->users().at(i)->login();
            out << _serv->users().at(i)->username();
            out << _serv->users().at(i)->groupId();
            out << _serv->users().at(i)->isAdmin();
        }
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_USERS_GET, block);
    }
        break;
    case CMSG_ADMIN_GROUPS_GET:
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (int)_serv->groups().size();
        for (int i = 0; i < _serv->groups().size(); i++)
        {
            out << _serv->groups().at(i)->id();
            out << _serv->groups().at(i)->name();
        }
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_GROUPS_GET, block);
    }
        break;
    case CMSG_ADMIN_TASKS_GET:
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (int)_serv->tasks().size();
        for (int i = 0; i < _serv->tasks().size(); i++)
        {
            out << (quint32)_serv->tasks().at(i)->id();
            out << (quint8)_serv->tasks().at(i)->type();
            out << _serv->tasks().at(i)->name();
            out << _serv->tasks().at(i)->content();
            out << _serv->tasks().at(i)->answer();
        }
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_TASKS_GET, block);
    }
        break;
    case CMSG_ADMIN_USER_DEL:
    {
        quint32 guid;
        in >> guid;
        _serv->delUser(_serv->searchUser(guid));
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_USER_DEL);
    }
        break;
    case CMSG_ADMIN_GROUP_DEL:
    {
        quint32 id;
        in >> id;
        _serv->delGroup(_serv->searchGroup(id));
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_GROUP_DEL);
    }
        break;
    case CMSG_ADMIN_TASK_DEL:
    {
        quint32 id;
        in >> id;
        _serv->delTask(_serv->searchTask(id));
        FlyNetwork::doSendPacket(_sok, SMSG_ADMIN_TASK_DEL);
    }
        break;
    case CMSG_TASK_SEND:
    {
        quint32 id; QString answer;
        in >> id;
        in >> answer;
        FlyAnswer *found_answer = searchAnswer(id);
        if (found_answer == NULL) {
            FlyAnswer *temp = new FlyAnswer(id, answer);
            _answers.append(temp);
        } else {
            found_answer->setAnswer(answer);
        }
    }
        break;
    case CMSG_SAVE_USERS_AND_GROUP:
    {
        FlyFactory::saveUsersAndGroups(_serv->users(), _serv->groups());
    }
        break;
    default:
    {
        qDebug() << "Receiver for this command not found!";
    }
        break;
    }
}

FlyAnswer* FlyClient::searchAnswer(quint32 id)
{
    for (int i = 0; i < _answers.size(); ++i) {
        if (_answers.at(i)->id() == id)
            return _answers.at(i);
    }
    return NULL;
}

void FlyClient::doSendAnswers()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << _answers.size();
    for (int i = 0; i < _answers.size(); i++) {
        out << (quint32)_answers.at(i)->id();
        out << _answers.at(i)->answer();
    }
    FlyNetwork::doSendPacket(_sok, SMSG_TASK_SEND, block);
}
