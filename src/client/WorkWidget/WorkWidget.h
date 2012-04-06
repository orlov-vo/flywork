/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef WORKWIDGET_H
#define WORKWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "FlyCore.h"

#include <QLabel>
#include <QPushButton>
#include <QFormLayout>

namespace Ui {
    class WorkWidget;
}

class WorkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkWidget(FlyCore *core, QWidget *parent = 0);
    void reloadWidget();

private slots:
    void doUpdateTime();
    void onListTasksItemChanged(QListWidgetItem *newitem, QListWidgetItem *olditem);

    void onUpdateWork();
    void onUpdateTasks();

    void onAnswerDone();
    void on_pbAnswer_clicked();

private:
    FlyCore *_core;
    Ui::WorkWidget *ui;

    QTimer *_timer;

    QLabel *lAnswer;
    QLineEdit *leAnswer;
    QPushButton *pbAnswer;
    QFormLayout *formAnswer;

    FlyTask *_currentTask;
};

#endif // WAITWIDGET_H
