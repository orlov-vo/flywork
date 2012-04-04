/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyCore.h"

#include "AdminWindow.h"
#include "AuthDialog.h"
#include "MainWindow.h"

#include <QApplication>
#include <QMessageBox>

FlyCore::FlyCore(QObject *parent) :
    QObject(parent)
{
    _sok = new QTcpSocket(this);
    connect(_sok, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(_sok, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(_sok, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

    _waitDisconnect = false;
}

void FlyCore::startSession()
{
    _user = new FlyUser();
    _work = new FlyWork();
    _tasks.clear();

    AuthDialog *w = new AuthDialog(this);
    w->show();
}

void FlyCore::closeSession()
{
    _waitDisconnect = true;
    _sok->disconnectFromHost();
    QApplication::closeAllWindows();
}

void FlyCore::loadMainWindow()
{
    MainWindow *w = new MainWindow(this);
    w->show();
}

void FlyCore::loadAdminWindow()
{
    AdminWindow *w = new AdminWindow(this);
    w->show();
}

void FlyCore::onSokConnected()
{
    _blockSize = 0;
    qDebug() << "Connected to "+_sok->peerAddress().toString()+":"+QString::number(_sok->peerPort());
    emit receivePacket(FMSG_AUTH_START);
}

void FlyCore::onSokDisconnected()
{
    if (!_waitDisconnect) {
        int ret = QMessageBox::critical(0,
                                       tr("Соединение потеряно..."),
                                       tr("Сервер разорвал соединение. Работа не будет сохранена, однако, Вы можете просмотреть её, либо перезайти и начать заного."),
                                       QMessageBox::Close | QMessageBox::Ignore,
                                       QMessageBox::Ignore);
        if (ret == QMessageBox::Close)
        {
            startSession();
        }
    }
    _waitDisconnect = false;
}

void FlyCore::onSokDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::warning(0, tr("Ошибка соединения"), tr("Сервер не найден!"), QMessageBox::Ok);
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::warning(0, tr("Ошибка соединения"), tr("Соединение было разорвано узлом."), QMessageBox::Ok);
        break;
    default:
        QMessageBox::critical(0, tr("Ошибка соединения"), tr("Произошла следующая ошибка: ")+_sok->errorString()), QMessageBox::Ok;
    }
}

void FlyCore::onSokReadyRead()
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

    emit receivePacket(command);
    _blockSize = 0;
}

FlyTask* FlyCore::searchTask(quint32 id)
{
    for (int i = 0; i < _tasks.count(); ++i)
    {
        if (_tasks.at(i)->id() == id)
        {
            return _tasks.at(i);
        }
    }
    return NULL;
}

void FlyCore::setWork(QString title, QTime endTime)
{
    this->work()->setTitle(title);
    this->work()->setEndTime(endTime);
    emit updateWork();
}

void FlyCore::setTasks(QList<FlyTask *> tasks)
{
    _tasks = tasks;
    emit updateTasks();
}

void FlyCore::setWorkStatus(fWork::WorkStatus status)
{
    this->work()->setStatus(status);
    emit updateWorkStatus(status);
}

void FlyCore::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}
