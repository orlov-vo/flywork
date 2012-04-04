/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyFactory.h"

QList<FlyUser *> FlyFactory::users()
{
    QList<FlyUser *> list;
    QString fileprefix = "flyfactory_users"; int count; QString t_fileprefix;
    quint32 guid; QString login; QString name; QByteArray hash; quint32 group; bool isAdmin;

    QFile file("./users.fdb");

    if (!file.exists()) {
        qWarning() << "File \"./user.fdb\" not found!";
        if (file.open(QIODevice::ReadWrite)) {
            FlyUser *admin = new FlyUser();
            admin->setGuid(1);
            admin->setLogin("admin");
            admin->setUsername("Administrator");
            admin->setHash(QCryptographicHash::hash("admin:password:flywR", QCryptographicHash::Sha1));
            admin->setGroupId(0);
            admin->setAdmin(true);

            QDataStream out(&file);
            out << fileprefix;
            out << (quint32)1;
            out << admin->guid();
            out << admin->login();
            out << admin->username();
            out << admin->hash();
            out << admin->groupId();
            out << admin->isAdmin();

            qDebug() << "Created new \"./users.fdb\"";
        } else {
            qFatal("Could not create file %s", file.fileName().toLatin1());
        }
    }

    if (file.isOpen() || file.open(QIODevice::ReadOnly)) {
        file.seek(0);
        QDataStream in(&file);
        in >> t_fileprefix;

        if (fileprefix == t_fileprefix) {
            in >> count;
            for (int i = 0; i < count; i++) {
                FlyUser *user = new FlyUser();
                in >> guid; user->setGuid(guid);
                in >> login; user->setLogin(login);
                in >> name; user->setUsername(name);
                in >> hash; user->setHash(hash);
                in >> group; user->setGroupId(group);
                in >> isAdmin; user->setAdmin(isAdmin);
                list.append(user);
            }
            qDebug("Loaded %u users from \"./users.fdb\"", count);
        } else {
            qDebug() << "flyfactory_users has error in fdb!";
        }
    }

    if (file.isOpen())
        file.close();

    return list;
}

QList<FlyTask *> FlyFactory::tasks()
{
    QList<FlyTask *> list;
    QString fileprefix = "flyfactory_work"; int count; QString t_fileprefix;
    QString title; QTime endTime;
    quint32 id; QString name; quint8 type; QString content; QString answer;

    QFile file("./work.fdb");
    if (file.open(QIODevice::ReadOnly)) {
        file.seek(0);
        QDataStream in(&file);
        in >> t_fileprefix;

        if (fileprefix == t_fileprefix) {
            in >> title; //temp
            in >> endTime; //temp
            in >> count;
            for (int i = 0; i < count; i++) {
                FlyTask *task = new FlyTask();
                in >> id; task->setId(id);
                in >> type; task->setType(static_cast<fTask::typeTask>(type));
                in >> name; task->setName(name);
                in >> content; task->setContent(content);
                in >> answer; task->setAnswer(answer);
                list.append(task);
            }
            qDebug("Loaded %u tasks from \"./work.fdb\"", count);
        } else {
            qDebug() << "flyfactory_work has error in fdb!";
        }
        file.close();
    } else {
        qWarning() << "File \"./work.fdb\" not found!";
    }

    return list;
}

QList<FlyGroup *> FlyFactory::groups()
{
    QList<FlyGroup *> list;
    QString fileprefix = "flyfactory_groups"; QString t_fileprefix; int count;
    quint32 id; QString name;

    QFile file("./groups.fdb");
    if (!file.exists()) {
        qWarning() << "File \"./groups.fdb\" not found!";
        if (file.open(QIODevice::ReadWrite)) {
            QDataStream out(&file);
            out << fileprefix;
            out << (quint32)0;
            file.close();
            qDebug() << "Created new \"./groups.fdb\"";
        } else {
            qFatal("Could not create file %s", file.fileName().toLatin1());
        }
    }

    if (file.isOpen() || file.open(QIODevice::ReadOnly)) {
        file.seek(0);
        QDataStream in(&file);
        in >> t_fileprefix;

        if (fileprefix == t_fileprefix) {
            in >> count;
            for (int i = 0; i < count; i++) {
                FlyGroup *group = new FlyGroup();
                in >> id; group->setId(id);
                in >> name; group->setName(name);
                list.append(group);
            }
            qDebug("Loaded %u groups from \"./groups.fdb\"", count);
        } else {
            qDebug() << "flyfactory_groups has error in fdb!";
        }
        file.close();
    }

    if (file.isOpen())
        file.close();

    return list;
}

FlyWork* FlyFactory::work()
{
    FlyWork *work = new FlyWork();
    QString fileprefix = "flyfactory_work"; QString t_fileprefix;
    QString title; QTime endTime;

    QFile file("./work.fdb");
    if (!file.exists()) {
        qWarning() << "File \"./work.fdb\" not found!";
        if (file.open(QIODevice::ReadWrite)) {
            QDataStream out(&file);
            out << fileprefix;
            out << work->title();
            out << work->endTime();
            out << (quint32)3;

            for (int i = 1; i < 4; i++) {
                FlyTask *task = new FlyTask();
                task->setId(i);
                task->setType(fTask::inputBox);
                task->setName(QObject::tr("Демо-задание ")+QString::number(i));
                task->setContent(QObject::tr("Тело задания"));
                task->setAnswer(QObject::tr("Ответ"));
                out << (quint32)task->id();
                out << (quint8)task->type();
                out << task->name();
                out << task->content();
                out << task->answer();
            }
            file.close();
            qDebug() << "Created new \"./work.fdb\"";
        } else {
            qFatal("Could not create file %s", file.fileName().toLatin1());
        }
    }

    if (file.isOpen() || file.open(QIODevice::ReadOnly)) {
        file.seek(0);
        QDataStream in(&file);
        in >> t_fileprefix;

        if (fileprefix == t_fileprefix) {
            in >> title; work->setTitle(title);
            in >> endTime; work->setEndTime(endTime);

            qDebug() << "Loaded work info from \"./work.fdb\"";
        } else {
            qDebug() << "flyfactory_work has error in fdb!";
        }
        file.close();
    }

    if (file.isOpen())
        file.close();

    return work;
}

void FlyFactory::saveWork(FlyWork *work, QList<FlyTask *> tasks)
{
    QString fileprefix = "flyfactory_work";

    QFile file("./work.fdb");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << fileprefix;
        out << work->title();
        out << work->endTime();
        out << (quint32)tasks.size();
        for (int i = 0; i < tasks.size(); i++) {
            out << tasks.at(i)->id();
            out << (quint8)tasks.at(i)->type();
            out << tasks.at(i)->name();
            out << tasks.at(i)->content();
            out << tasks.at(i)->answer();
        }
        file.close();
        qDebug() << "Successful saving work and task to \"./work.fdb\"";
    } else {
        qFatal("Could not create file %s", file.fileName().toLatin1());
    }
}
