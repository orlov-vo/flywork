/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include <QCryptographicHash>
#include <QTimer>
#include <QMessageBox>

#include "FlyCore.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit AdminWindow(FlyCore * core, QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);
    
private slots:
    void onReceivePacket(quint16 opcode);

    void groups();
    void users();
    void tasks();

    void on_pbSave_clicked();

    void onTableUsersItemChanged(QTableWidgetItem *newitem, QTableWidgetItem *olditem);
    void onListGroupItemChanged(QListWidgetItem *newitem, QListWidgetItem *olditem);
    void onComboBoxTasksItemChanged(int index);

    void on_pbWorkStatus_clicked();
    void on_pbRefresh_clicked();

    void on_pbUserSave_clicked();
    void on_pbGroupSave_clicked();
    void on_pbTaskSave_clicked();

    void on_pbUserDelete_clicked();
    void on_pbGroupDelete_clicked();
    void on_pbTaskDelete_clicked();



private:
    Ui::AdminWindow *ui;
    FlyCore * _core;
    QTcpSocket *_sok;
};

#endif // ADMINWINDOW_H
