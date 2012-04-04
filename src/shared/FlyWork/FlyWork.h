/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYWORK_H
#define FLYWORK_H

#include <QObject>
#include <QTime>

#include "Common.h"

class FlyWork : public QObject
{
    Q_OBJECT
public:
    explicit FlyWork(QObject *parent = 0);
    
    void setTitle(QString title) { _title = title; }
    QString title() { return _title; }
    void setEndTime(QTime endTime) { _endTime = endTime; }
    QTime endTime() { return _endTime; }
    void setStatus(fWork::WorkStatus status) { _status = status; }
    fWork::WorkStatus status() { return _status; }
    void setValid(bool valid) { _isValid = valid; }
    bool isValid() { return _isValid; }

private:
    QString _title;
    QTime _endTime;

    fWork::WorkStatus _status;
    bool _isValid;
};

#endif // FLYWORK_H
