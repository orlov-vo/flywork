/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>

#include "FlyCore.h"

class FlyNetwork;

namespace Ui {
    class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(FlyCore *core, QWidget *parent = 0);
    ~AuthDialog();

private slots:
    void onSokDisplayError();
    void onReceivePacket(quint16 opcode);

    void on_pbAuth_clicked();

private:
    Ui::AuthDialog *ui;
    FlyCore *_core;
    QTcpSocket *_sok;
};

#endif // AUTHDIALOG_H
