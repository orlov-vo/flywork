/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "AdminWindow.h"
#include "ui_AdminWindow.h"

AdminWindow::AdminWindow(FlyCore * core, QWidget *parent) :
    QMainWindow(parent), _core(core), ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    _sok = _core->socket();

    connect(_core, SIGNAL(receivePacket(quint16)), this, SLOT(onReceivePacket(quint16)));
    connect(ui->tableUsers, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(onTableUsersItemChanged(QTableWidgetItem*,QTableWidgetItem*)));
    connect(ui->listGroup, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(onListGroupItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->cbTaskId, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxTasksItemChanged(int)));
    ui->leWorkTitle->setText(_core->work()->title());
    ui->teEndTime->setTime(_core->work()->endTime());

    QListWidgetItem *group_none = new QListWidgetItem(tr("- Новая группа -"));
    group_none->setData(32, (quint32)0);

    ui->listGroup->addItem(group_none);

    ui->cbGroup->addItem(tr("- Без группы -"), 0);

    QListWidgetItem *workstatus_pause = new QListWidgetItem(tr("Работа приостоновленна"));
    workstatus_pause->setData(32,fWork::statusPause);
    QListWidgetItem *workstatus_start = new QListWidgetItem(tr("Работа начата"));
    workstatus_start->setData(32,fWork::statusStart);
    QListWidgetItem *workstatus_stop = new QListWidgetItem(tr("Работа закончена"));
    workstatus_stop->setData(32,fWork::statusStop);

    ui->lwWorkStatus->addItem(workstatus_pause);
    ui->lwWorkStatus->addItem(workstatus_start);
    ui->lwWorkStatus->addItem(workstatus_stop);

    switch (_core->work()->status()) {
    case fWork::statusPause:
        ui->lwWorkStatus->setItemSelected(workstatus_pause, true);
        break;
    case fWork::statusStart:
        ui->lwWorkStatus->setItemSelected(workstatus_start, true);
        break;
    case fWork::statusStop:
        ui->lwWorkStatus->setItemSelected(workstatus_stop, true);
        break;
    }

    ui->cbTaskId->insertItem(0, tr("- Новое задание -"), 0);

    ui->cbTaskType->insertItem(0, tr("Input Box"), fTask::inputBox);
    ui->cbTaskType->insertItem(1, tr("Radio Box"), fTask::radioBox);
    ui->cbTaskType->insertItem(2, tr("Check Box"), fTask::checkBox);
    ui->cbTaskType->insertItem(3, tr("Temp File"), fTask::tempFile);

    ui->tableUsers->setColumnCount(5);
    ui->tableUsers->setColumnWidth(0, 40);
    ui->tableUsers->setColumnWidth(1, 140);
    ui->tableUsers->setColumnWidth(2, 219);
    ui->tableUsers->setColumnWidth(3, 100);
    ui->tableUsers->setColumnWidth(4, 90);

    QTableWidgetItem *label_guid = new QTableWidgetItem(tr("GUID"));
    QTableWidgetItem *label_login = new QTableWidgetItem(tr("Логин"));
    QTableWidgetItem *label_name = new QTableWidgetItem(tr("Имя"));
    QTableWidgetItem *label_groupid = new QTableWidgetItem(tr("ID Группы"));
    QTableWidgetItem *label_isadmin = new QTableWidgetItem(tr("Права админа"));

    ui->tableUsers->setHorizontalHeaderItem(0, label_guid);
    ui->tableUsers->setHorizontalHeaderItem(1, label_login);
    ui->tableUsers->setHorizontalHeaderItem(2, label_name);
    ui->tableUsers->setHorizontalHeaderItem(3, label_groupid);
    ui->tableUsers->setHorizontalHeaderItem(4, label_isadmin);

    QTimer::singleShot(100, this, SLOT(groups()));
    QTimer::singleShot(200, this, SLOT(users()));
    QTimer::singleShot(300, this, SLOT(tasks()));
}

void AdminWindow::closeEvent(QCloseEvent *)
{
    disconnect(_core, SIGNAL(receivePacket(quint16)), this, SLOT(onReceivePacket(quint16)));
    disconnect(ui->tableUsers, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(onTableUsersItemChanged(QTableWidgetItem*,QTableWidgetItem*)));
    disconnect(ui->listGroup, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(onListGroupItemChanged(QListWidgetItem*,QListWidgetItem*)));
    disconnect(ui->cbTaskId, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxTasksItemChanged(int)));
    delete ui;
}

void AdminWindow::onReceivePacket(quint16 opcode)
{
    QDataStream in(_sok);

    switch (opcode) {
    case SMSG_ADMIN_USER_SAVE:
    case SMSG_ADMIN_USER_DEL:
    {
        users();
    }
        break;
    case SMSG_ADMIN_GROUP_SAVE:
    case SMSG_ADMIN_GROUP_DEL:
    {
        groups();
    }
        break;
    case SMSG_ADMIN_TASK_SAVE:
    case SMSG_ADMIN_TASK_DEL:
    {
        tasks();
    }
        break;
    case SMSG_ADMIN_USERS_GET:
    {
        int count;
        quint32 guid; QString login; QString name; quint32 groupid; bool isAdmin;
        in >> count;

        ui->tableUsers->setRowCount(count);
        for (int i = 0; i < count; i++) {
            in >> guid;
            in >> login;
            in >> name;
            in >> groupid;
            in >> isAdmin;

            QTableWidgetItem *i_guid = new QTableWidgetItem();
            i_guid->setText(QString::number(guid));
            i_guid->setData(32, guid);
            QTableWidgetItem *i_login = new QTableWidgetItem(login);
            i_login->setText(login);
            i_login->setData(32, login);
            QTableWidgetItem *i_name = new QTableWidgetItem(name);
            i_name->setText(name);
            i_name->setData(32, name);
            QTableWidgetItem *i_groupid = new QTableWidgetItem();
            i_groupid->setText(QString::number(groupid));
            i_groupid->setData(32, groupid);
            QTableWidgetItem *i_isAdmin = new QTableWidgetItem();
            if(isAdmin)
                i_isAdmin->setText(tr("Да"));
            else
                i_isAdmin->setText(tr("Нет"));
            i_isAdmin->setData(32, isAdmin);

            ui->tableUsers->setItem(i, 0, i_guid);
            ui->tableUsers->setItem(i, 1, i_login);
            ui->tableUsers->setItem(i, 2, i_name);
            ui->tableUsers->setItem(i, 3, i_groupid);
            ui->tableUsers->setItem(i, 4, i_isAdmin);
        }
    }
        break;
    case SMSG_ADMIN_GROUPS_GET:
    {
        int count;
        quint32 id; QString name;
        in >> count;
        ui->cbGroup->clear();
        ui->cbGroup->insertItem(0, tr("- Без группы -"), (quint32)0);

        ui->listGroup->clear();
        QListWidgetItem *group_none = new QListWidgetItem(tr("- Новая группа -"));
        group_none->setData(32, (quint32)0);
        ui->listGroup->addItem(group_none);
        for(int i = 0; i < count; i++)
        {
            int idx = i++;
            in >> id;
            in >> name;
            QListWidgetItem *item = new QListWidgetItem(name);
            item->setData(32, id);
            ui->listGroup->addItem(item);
            ui->cbGroup->insertItem(idx, name, id);
        }
    }
        break;
    case SMSG_ADMIN_TASKS_GET:
    {
        int count;
        quint32 id; quint8 type; QString name; QString desc; QString ans;
        in >> count;
        ui->cbTaskId->clear();
        ui->cbTaskId->insertItem(0, tr("- Новое задание -"), 0);
        for(int i = 0; i < count; i++)
        {
            int idx = i+1;
            in >> id;
            in >> type;
            in >> name;
            in >> desc;
            in >> ans;
            ui->cbTaskId->insertItem(idx, name, id);
            ui->cbTaskId->setItemData(idx, type, 33);
            ui->cbTaskId->setItemData(idx, name, 34);
            ui->cbTaskId->setItemData(idx, desc, 35);
            ui->cbTaskId->setItemData(idx, ans, 36);
        }
    }
        break;
    default:
    {
        qDebug() << "Function for opcode " + LookupOpcodeName(opcode) + " not found in AdminWindow";
    }
        break;
    }
}

void AdminWindow::onTableUsersItemChanged(QTableWidgetItem *newitem, QTableWidgetItem * olditem)
{
    if (olditem && (newitem->row() == olditem->row()))
        return;
    ui->sbGUID->setValue(ui->tableUsers->item(newitem->row(), 0)->data(32).toInt());
    ui->leLogin->setText(ui->tableUsers->item(newitem->row(), 1)->data(32).toString());
    ui->leUsername->setText(ui->tableUsers->item(newitem->row(), 2)->data(32).toString());
    ui->lePassword->setText(tr("<SHA1-HASH>"));

    for (int i = 0; i < ui->listGroup->count(); i++) {
        if (ui->tableUsers->item(newitem->row(), 3)->data(32) == ui->cbGroup->itemData(i))
            ui->cbGroup->setCurrentIndex(i);
    }

    if (ui->tableUsers->item(newitem->row(), 4)->data(32).toBool())
        ui->cbIsAdmin->setCheckState(Qt::Checked);
    else
        ui->cbIsAdmin->setCheckState(Qt::Unchecked);
}

void AdminWindow::onListGroupItemChanged(QListWidgetItem *newitem, QListWidgetItem * olditem)
{
    if (!newitem || newitem == olditem)
        return;
    if (newitem->data(32) == 0) {
        ui->leGroupName->setText("");
        ui->sbGroupID->setValue(0);
    } else {
        ui->leGroupName->setText(newitem->text());
        ui->sbGroupID->setValue(newitem->data(32).toInt());
    }
}

void AdminWindow::onComboBoxTasksItemChanged(int index)
{
    for (int i = 0; i < ui->cbTaskType->count(); i++)
        if (ui->cbTaskId->itemData(index, 33) == ui->cbTaskType->itemData(i))
            ui->cbTaskType->setCurrentIndex(i);

    ui->leTaskName->setText(ui->cbTaskId->itemData(index, 34).toString());
    ui->teTaskDesc->setHtml(ui->cbTaskId->itemData(index, 35).toString());
    ui->leTaskTrue->setText(ui->cbTaskId->itemData(index, 36).toString());
}

void AdminWindow::groups()
{
    FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_GROUPS_GET);
}

void AdminWindow::users()
{
    FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_USERS_GET);
}

void AdminWindow::tasks()
{
    FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_TASKS_GET);
}

void AdminWindow::on_pbSave_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << ui->leWorkTitle->text();
    out << ui->teEndTime->time();
    FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_INFO_SAVE, block);
}

void AdminWindow::on_pbWorkStatus_clicked()
{
    QByteArray block;
    block.append((quint8)ui->lwWorkStatus->currentItem()->data(32).toUInt());
    FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_WORK_PROCESS, block);
}

void AdminWindow::on_pbRefresh_clicked()
{
    QTimer::singleShot(100, this, SLOT(groups()));
    QTimer::singleShot(200, this, SLOT(users()));
}

void AdminWindow::on_pbUserSave_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << ui->sbGUID->value();
    out << ui->leLogin->text();
    out << ui->leUsername->text();
    if (ui->lePassword->text() != "<SHA1-HASH>") {
        out << true;
        QByteArray str;
        str.append(ui->leLogin->text()+":"+ui->lePassword->text()+":flywR");
        out << QCryptographicHash::hash(str, QCryptographicHash::Sha1);
    } else {
        out << false;
    }

    out << (quint32)ui->cbGroup->itemData(ui->cbGroup->currentIndex()).toUInt();

    if (ui->cbIsAdmin->checkState() == Qt::Checked)
        out << true;
    else
        out << false;
    FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_USER_SAVE, block);
}

void AdminWindow::on_pbGroupSave_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << ui->sbGroupID->value();
    out << ui->leGroupName->text();
    FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_GROUP_SAVE, block);
}

void AdminWindow::on_pbTaskSave_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint32)ui->cbTaskId->itemData(ui->cbTaskId->currentIndex()).toUInt();
    out << (quint8)ui->cbTaskType->itemData(ui->cbTaskType->currentIndex()).toUInt();
    out << ui->leTaskName->text();
    out << ui->teTaskDesc->toPlainText();
    out << ui->leTaskTrue->text();
    FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_TASK_SAVE, block);
}

void AdminWindow::on_pbUserDelete_clicked()
{
    int ret = QMessageBox::warning(
                this,
                tr("Удаление пользователя"),
                tr("Вы желаете удалить пользователя \"%1\" (id: %2)?").arg(ui->leUsername->text(), QString::number(ui->sbGUID->value())),
                QMessageBox::Yes | QMessageBox::Cancel,
                QMessageBox::Cancel);
    if (ret == QMessageBox::Yes) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << ui->sbGUID->value();
        FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_USER_DEL, block);
    }
}

void AdminWindow::on_pbGroupDelete_clicked()
{
    int ret = QMessageBox::warning(
                this,
                tr("Удаление группы"),
                tr("Вы желаете удалить группу \"%1\" (id: %2)?").arg(ui->leGroupName->text(), QString::number(ui->sbGroupID->value())),
                QMessageBox::Yes | QMessageBox::Cancel,
                QMessageBox::Cancel);
    if (ret == QMessageBox::Yes) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << ui->sbGroupID->value();
        FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_GROUP_DEL, block);
    }
}

void AdminWindow::on_pbTaskDelete_clicked()
{
    int ret = QMessageBox::warning(
                this,
                tr("Удаление задания"),
                tr("Вы желаете удалить задание \"%1\" (id: %2)?").arg(ui->leTaskName->text(), QString::number(ui->cbTaskId->currentIndex())),
                QMessageBox::Yes | QMessageBox::Cancel,
                QMessageBox::Cancel);
    if (ret == QMessageBox::Yes) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint32)ui->cbTaskId->itemData(ui->cbTaskId->currentIndex()).toUInt();
        FlyNetwork::doSendPacket(_sok, CMSG_ADMIN_TASK_DEL, block);
    }
}

void AdminWindow::on_pbSaveToDB_clicked()
{
    int ret = QMessageBox::information(
                this,
                tr("Сохранение в Базу Данных"),
                tr("Вы желаете перезаписать на сервере данные пользователей и групп в соответствии с текущим положением?"),
                QMessageBox::Yes | QMessageBox::Cancel,
                QMessageBox::Cancel);
    if (ret == QMessageBox::Yes)
        FlyNetwork::doSendPacket(_sok, CMSG_SAVE_USERS_AND_GROUP);
}
