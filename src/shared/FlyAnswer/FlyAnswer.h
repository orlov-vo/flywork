/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYANSWER_H
#define FLYANSWER_H

#include <QObject>

#include "Common.h"

class FlyAnswer : public QObject
{
    Q_OBJECT
public:
    explicit FlyAnswer(quint32 id, QString answer, QObject *parent = 0);
    
    void setId(quint32 id);
    void setAnswer(QString text);

    quint32 id() { return _id; }
    QString answer() { return _answer; }

signals:
    void answerChanged(QString answer);

private:
    quint32 _id;
    QString _answer;
    
};

#endif // FLYANSWER_H
