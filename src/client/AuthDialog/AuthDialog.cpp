/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "AuthDialog.h"
#include "ui_AuthDialog.h"

#include <QtGui>
#include <QDebug>

AuthDialog::AuthDialog(FlyCore * core, QWidget *parent) :
    QDialog(parent), ui(new Ui::AuthDialog), _core(core)
{
    ui->setupUi(this);
    _sok = _core->socket();

    connect(_sok, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSokDisplayError()));
    connect(_core, SIGNAL(receivePacket(quint16)), this, SLOT(onReceivePacket(quint16)));
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::onSokDisplayError()
{
    ui->pbAuth->setEnabled(true);
}

void AuthDialog::onReceivePacket(quint16 opcode)
{
    switch (opcode) {
    case FMSG_AUTH_START:
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        _core->user()->setLogin(ui->leName->text());
        out << _core->user()->login();
        FlyNetwork::doSendPacket(_sok, CMSG_AUTH_USERNAME, block);
    }
        break;
    case SMSG_AUTH_USERNAME_INVALID:
    {
        _core->waitDisconnect();
        QMessageBox::information(this, tr("Ошибка"), tr("Данное имя недопустимое."));
        ui->pbAuth->setEnabled(true);
    }
        break;
    case SMSG_AUTH_USERNAME_USED:
    {
        _core->waitDisconnect();
        QMessageBox::information(this, tr("Ошибка"), tr("Данный аккаунт уже используется."));
        ui->pbAuth->setEnabled(true);
    }
        break;
    case SMSG_AUTH_PASSWORD_INVALID:
    {
        _core->waitDisconnect();
        QMessageBox::information(this, tr("Ошибка"), tr("Данный пароль недопустим."));
        ui->pbAuth->setEnabled(true);
    }
        break;
    case SMSG_AUTH_CHALLENGE:
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        QByteArray str;
        str.append(ui->leName->text()+":"+ui->lePassword->text()+":flywR");
        out << QCryptographicHash::hash(str, QCryptographicHash::Sha1);
        FlyNetwork::doSendPacket(_sok, CMSG_AUTH_PASSWORD, block);
    }
        break;
    case SMSG_AUTH_SUCCESS:
    {
        if (this->close()) {
            disconnect(_core, 0, this, 0);
            _core->loadMainWindow();
        }
    }
        break;
    default:
    {
        qDebug() << "Function for opcode " + LookupOpcodeName(opcode) + " not found in AuthDialog";
    }
        break;
    }
}

void AuthDialog::on_pbAuth_clicked()
{
    ui->pbAuth->setEnabled(false);
    QString Host = "127.0.0.1";
    quint16 Port = 6425;
    _sok->connectToHost(Host, Port);
}
