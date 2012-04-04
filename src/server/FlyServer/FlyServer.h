/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYSERVER_H
#define FLYSERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QTimer>
#include "FlyClient.h"
#include "FlyFactory.h"

#include "FlyNetwork.h"

class FlyClient;
class FlyNetwork;
class FlyWork;

class FlyServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit FlyServer(QObject *parent = 0);
    bool doStartServer(QHostAddress addr, qint16 port);
    void doSendToAllServerMessage(QString message);
    void doSendServerMessageToUsers(QString message, const QStringList &users);
    QStringList getUsersOnline() const;

    void doSendWorkInfo();
    void doSendWorkInfo(FlyClient *client);

    void doSendToAllPacket(quint16 command, QByteArray block);
    void doSendToAllPacket(quint16 command);

    void addUser(FlyUser *user) { _users.append(user); }
    void addGroup(FlyGroup *group) { _groups.append(group); }
    void addTask(FlyTask *task) { _tasks.append(task); }

    void delUser(FlyUser *user) { for(int i=0; i<_users.size();i++) if(user == _users.at(i)) _users.removeAt(i); }
    void delGroup(FlyGroup *group) { for(int i=0; i<_groups.size();i++) if(group == _groups.at(i)) _groups.removeAt(i); }
    void delTask(FlyTask *task) { for(int i=0; i<_tasks.size();i++) if(task == _tasks.at(i)) _tasks.removeAt(i); }

    bool isNameValid(QString name) const;
    bool isNameUsed(QString name) const;
    bool isPasswordTrue(FlyUser *user, QByteArray hash);

    FlyUser* searchUser(quint32 guid);
    FlyUser* searchUser(QString name);
    FlyGroup* searchGroup(quint32 id);
    FlyTask* searchTask(quint32 id);

    FlyWork* work() { return _work; }
    QList<FlyUser *> users() { return _users; }
    QList<FlyTask *> tasks() { return _tasks; }
    QList<FlyGroup *> groups() { return _groups; }

    void setWork(QString title, QTime endTime);
    void setTasks(QList<FlyTask *> tasks);
    void setWorkStatus(fWork::WorkStatus status);

signals:
    void updateWork();
    void updateTasks();
    void updateWorkStatus(fWork::WorkStatus status);

public slots:
    void onRemoveUser(FlyClient *client);

protected:
    void incomingConnection(int handle);

private slots:
    void doUpdateTime();
//    void onUpdateWorkStatus(fWork::WorkStatus);

private:
    QList<FlyClient *> _clients;
    QList<FlyUser *> _users;
    QList<FlyTask *> _tasks;
    QList<FlyGroup *> _groups;
    FlyWork *_work;

    QTimer *_timer;
};



#endif // FLYSERVER_H
