/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "MainWindow.h"

#include <QtGui>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(FlyCore *core, QWidget *parent) :
    QMainWindow(parent), _core(core)
{
    _sok = _core->socket();

    connect(_core, SIGNAL(receivePacket(quint16)), this, SLOT(onReceivePacket(quint16)));
    connect(_core, SIGNAL(updateWorkStatus(fWork::WorkStatus)), this, SLOT(onUpdateWorkStatus(fWork::WorkStatus)));

    QMenu *mainMenu = this->menuBar()->addMenu(tr("&Главное"));
    mainMenu->addAction(tr("Заврешить сеанс"), this, SLOT(actionDisconnect()));
    mainMenu->addAction(tr("Выход из программы"), this, SLOT(actionQuit()));

    setVisibleElements(false);

    this->setWindowTitle(tr("FlyWork - Рабочее окружение"));
    this->setWindowIcon(QIcon(":/authdialog/images/flywork_client.png"));
    this->setMinimumSize(600, 400);

    FlyNetwork::doSendPacket(_sok, CMSG_USER_INFO);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    disconnect(_core, SIGNAL(receivePacket(quint16)), this, SLOT(onReceivePacket(quint16)));
    event->accept();
}

void MainWindow::setVisibleElements(bool visible)
{
    if (visible) {
        _workWidget = new WorkWidget(_core);
        this->setCentralWidget(_workWidget);
        _workWidget->reloadWidget();
    } else {
        _waitWidget = new WaitWidget(_core);
        this->setCentralWidget(_waitWidget);
    }
}

void MainWindow::onReceivePacket(quint16 opcode)
{
    QDataStream in(_sok);
    switch(opcode) {
    case SMSG_WORK_INFO:
    {
        QString title; QTime endTime; int count;
        quint32 id; quint8 type; QString name; QString content; QString answer;
        _core->tasks().clear();
        in >> title;
        in >> endTime;
        _core->setWork(title, endTime);
        in >> count;
        QList <FlyTask *> tasks;
        for (int i = 0; i < count; i++) {
            FlyTask *task = new FlyTask();
            in >> id; task->setId(id);
            in >> type; task->setType(static_cast<fTask::typeTask>(type));
            in >> name; task->setName(name);
            in >> content; task->setContent(content);
            in >> answer; task->setAnswer(answer);
            tasks.append(task);
        }
        _core->setTasks(tasks);
        _core->work()->setValid(true);
    }
        break;
    case SMSG_WORK_STATUS:
    {
        quint8 status;
        in >> status;
        emit _core->setWorkStatus(static_cast<fWork::WorkStatus>(status));

        if (status == fWork::statusStart || status == fWork::statusPause)
            FlyNetwork::doSendPacket(_sok, CMSG_WORK_INFO);
    }
        break;
    case SMSG_USER_INFO:
    {
        quint32 guid; QString name; quint32 groupid; QString group; bool admin;
        in >> guid; _core->user()->setGuid(guid);
        in >> name; _core->user()->setUsername(name);
        in >> groupid; _core->user()->setGroupId(groupid);
        in >> group; _core->user()->setGroupName(group);
        in >> admin; _core->user()->setAdmin(admin);

        if (admin == true) {
            this->menuBar()->addAction(tr("Панель администратора"), this, SLOT(actionAdminPanel()));
        }
        FlyNetwork::doSendPacket(_sok, CMSG_WORK_STATUS);
    }
        break;
    case SMSG_TASK_SEND:
    {
        int count;
        in >> count;
        _core->clearAnswers();
        for (int i = 0; i < count; ++i) {
            quint32 id; QString answer;
            in >> id;
            in >> answer;
            FlyAnswer *temp = new FlyAnswer(id, answer);
            _core->addAnswer(temp);
        }
    }
        break;
    default:
    {
        qDebug() << "Function for opcode " + LookupOpcodeName(opcode) + " not found in MainWindow";
    }
        break;
    }
}

void MainWindow::onUpdateWorkStatus(fWork::WorkStatus status)
{
    if (status == fWork::statusStart)
        setVisibleElements(true);
    else
        setVisibleElements(false);
}

void MainWindow::actionAdminPanel()
{
    _core->loadAdminWindow();
}

void MainWindow::actionDisconnect()
{
    _core->closeSession();
    _core->startSession();
}

void MainWindow::actionQuit()
{
    _core->closeSession();
    QApplication::exit();
}




