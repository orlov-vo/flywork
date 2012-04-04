/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>

#include "FlyCore.h"
#include "WaitWidget.h"
#include "WorkWidget.h"

class FlyNetwork;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(FlyCore *core, QWidget *parent = 0);
    
public slots:
    void onReceivePacket(quint16 opcode);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void actionAdminPanel();
    void actionDisconnect();
    void actionQuit();

    void onUpdateWorkStatus(fWork::WorkStatus status);

private:
    void setVisibleElements(bool visible);

    FlyCore *_core;
    QVBoxLayout *_layout;
    QMenuBar *_menuBar;
    WaitWidget *_waitWidget;
    WorkWidget *_workWidget;
    QTcpSocket *_sok;

};

#endif // MAINWINDOW_H
