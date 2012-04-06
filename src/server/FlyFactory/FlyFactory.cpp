/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyFactory.h"

QList<FlyUser *> FlyFactory::users()
{
    QList<FlyUser *> list;
    QByteArray fileprefix = "flyfactory_users"; int count; QByteArray t_fileprefix;
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
            qWarning("Could not create file %s", file.fileName().toAscii());
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
    QByteArray fileprefix = "flyfactory_work"; int count; QByteArray t_fileprefix;
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

QList<FlyAnswer *> FlyFactory::answers(FlyWork *work, FlyUser *user)
{
    QList<FlyAnswer *> list;
    if (!QDir("./work_" + work->id()).exists())
        return list;

    QByteArray fileprefix = "flywork_" + user->login().toAscii();
    QByteArray t_fileprefix;
    QString title; QByteArray id;
    quint32 guid; QString login; QString username; quint32 groupId; QString groupName; bool isAdmin;
    int count_tasks; int count_answers;

    QFile file("./work_" + work->id() + "/" + user->login().toAscii() + ".fuw");
    if (file.open(QIODevice::ReadOnly)) {
        file.seek(0);
        QDataStream in(&file);
        in >> t_fileprefix;

        if (fileprefix == t_fileprefix) {
            in >> guid;
            in >> login;
            in >> username;
            in >> groupId;
            in >> groupName;
            in >> isAdmin;
            in >> id;
            in >> title;
            in >> count_tasks;
            for (int i = 0; i < count_tasks; i++) {
                quint32 id; quint8 type; QString name; QString content; QString answer;
                in >> id;
                in >> type;
                in >> name;
                in >> content;
                in >> answer;
            }
            in >> count_answers;
            for (int i = 0; i < count_answers; i++) {
                quint32 id; QString answer;
                in >> id;
                in >> answer;
                FlyAnswer *temp = new FlyAnswer(id, answer);
                list.append(temp);
            }
            qDebug("Loaded %u answers from \"%s\"", list.size(), file.fileName().toAscii());
        } else {
            qDebug("%s has error in prefix file!", file.fileName().toAscii());
        }
        file.close();
    } else {
        qWarning("File \"%s\" not found!", file.fileName().toAscii());
    }

    return list;
}

QList<FlyGroup *> FlyFactory::groups()
{
    QList<FlyGroup *> list;
    QByteArray fileprefix = "flyfactory_groups"; QByteArray t_fileprefix; int count;
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
            qWarning("Could not create file %s", file.fileName().toAscii());
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
    QByteArray fileprefix = "flyfactory_work"; QByteArray t_fileprefix;
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
            qWarning("Could not create file %s", file.fileName().toAscii());
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
    QByteArray fileprefix = "flyfactory_work";

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
        qWarning("Could not create file %s", file.fileName().toAscii());
    }
}

void FlyFactory::saveUsersAndGroups(QList<FlyUser *> users, QList<FlyGroup *> groups)
{
    QByteArray fileprefix_groups = "flyfactory_groups";

    QFile file_groups("./groups.fdb");
    if (file_groups.open(QIODevice::WriteOnly)) {
        QDataStream out(&file_groups);
        out << fileprefix_groups;
        out << groups.size();
        for (int i = 0; i < groups.size(); i++) {
            out << groups.at(i)->id();
            out << groups.at(i)->name();
        }
        file_groups.close();
        qDebug() << "Successful saving groups to \"./groups.fdb\"";
    } else {
        qWarning("Could not create file %s", file_groups.fileName().toAscii());
    }

    QByteArray fileprefix_users = "flyfactory_users";

    QFile file_users("./users.fdb");
    if (file_users.open(QIODevice::WriteOnly)) {
        QDataStream out(&file_users);
        out << fileprefix_users;
        out << users.size();
        for (int i = 0; i < users.size(); i++) {
            out << users.at(i)->guid();;
            out << users.at(i)->login();
            out << users.at(i)->username();
            out << users.at(i)->hash();
            out << users.at(i)->groupId();
            out << users.at(i)->isAdmin();
        }
        file_users.close();
        qDebug() << "Successful saving users to \"./users.fdb\"";
    } else {
        qWarning("Could not create file %s", file_users.fileName().toAscii());
    }
}

void FlyFactory::saveTasks(FlyWork *work, QList<FlyTask *> tasks, FlyUser *user, QList<FlyAnswer *> answers)
{
    if (!QDir("./work_" + work->id()).exists())
        QDir().mkdir("./work_" + work->id());

    QByteArray fileprefix = "flywork_" + user->login().toAscii();

    QFile file("./work_" + work->id() + "/" + user->login().toAscii() + ".fuw");
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out << fileprefix;
        out << user->guid();
        out << user->login();
        out << user->username();
        out << user->groupId();
        out << user->groupName();
        out << user->isAdmin();
        out << work->id();
        out << work->title();
        out << (quint32)tasks.size();
        for (int i = 0; i < tasks.size(); i++) {
            out << tasks.at(i)->id();
            out << (quint8)tasks.at(i)->type();
            out << tasks.at(i)->name();
            out << tasks.at(i)->content();
            out << tasks.at(i)->answer();
        }
        for (int i = 0; i < answers.size(); i++) {
            out << answers.at(i)->id();
            out << answers.at(i)->answer();
        }

    } else {
        qWarning("Could not create file %s", file.fileName().toAscii());
    }
}
