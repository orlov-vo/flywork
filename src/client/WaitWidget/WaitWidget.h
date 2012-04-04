/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QWidget>
#include "Common.h"
#include "FlyCore.h"

namespace Ui {
    class WaitWidget;
}

class WaitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaitWidget(FlyCore *core, QWidget *parent = 0);

private slots:
    void onUpdateWorkStatus(fWork::WorkStatus);

private:
    Ui::WaitWidget *ui;
    FlyCore *_core;
};

#endif // WAITWIDGET_H
