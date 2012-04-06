/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYCORE_H
#define FLYCORE_H

#include <QObject>
#include <QTcpSocket>
#include <QLayout>

#include "Common.h"
#include "FlyNetwork.h"
#include "Opcodes.h"

#include "FlyAnswer.h"
#include "FlyUser.h"
#include "FlyWork.h"
#include "FlyTask.h"

class FlyCore : public QObject
{
    Q_OBJECT
public:
    explicit FlyCore(QObject *parent = 0);
    void startSession();
    void closeSession();
    void loadMainWindow();
    void loadAdminWindow();

    void setWork(QString title, QTime endTime);
    void setTasks(QList<FlyTask *> tasks);
    void setWorkStatus(fWork::WorkStatus status);

    FlyUser* user() const { return _user; }
    FlyWork* work() const { return _work; }
    QList<FlyTask *> tasks() const { return _tasks; }
    QList<FlyAnswer *> answers() const { return _answers; }
    QTcpSocket *socket() const { return _sok; }

    FlyTask* searchTask(quint32 id);
    FlyAnswer* searchAnswer(quint32 id);

    void addAnswer(FlyAnswer *answer) { _answers.append(answer); }
    void clearAnswers() { _answers.clear(); }

    void clearLayout(QLayout *layout);
    void waitDisconnect() { _waitDisconnect = true; }

private slots:
    void onSokConnected();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);
    void onSokReadyRead();
    void onSokDisconnected();

signals:
    void receivePacket(quint16 opcode);
    void updateWork();
    void updateTasks();
    void updateWorkStatus(fWork::WorkStatus status);

private:
    FlyUser *_user;
    FlyWork *_work;
    QList<FlyTask *> _tasks;
    QList<FlyAnswer *> _answers;
    QTcpSocket *_sok;
    quint16 _blockSize;

    bool _waitDisconnect;

};

#endif // FLYCORE_H
