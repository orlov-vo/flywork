/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYTASK_H
#define FLYTASK_H

#include <QObject>

#include "Common.h"

class FlyTask : public QObject
{
    Q_OBJECT
public:
    explicit FlyTask(QObject *parent = 0);
    
    void setId(quint32 id) { _id = id; }
    void setType(fTask::typeTask type) { _type = type; }
    void setName(QString name) { _name = name; }
    void setContent(QString text) { _content = text; }
    void setAnswer(QString text) { _answer = text; }

    quint32 id() { return _id; }
    fTask::typeTask type() { return _type; }
    QString name() { return _name; }
    QString content() { return _content; }
    QString answer() { return _answer; }

private:
    quint32 _id;
    fTask::typeTask _type;
    QString _name;
    QString _content;
    QString _answer;
    
};

#endif // FLYTASK_H
