/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#ifndef FLYGROUP_H
#define FLYGROUP_H

#include <QObject>

class FlyGroup : public QObject
{
    Q_OBJECT
public:
    explicit FlyGroup(QObject *parent = 0);
    
    void setId(quint32 id) { _id = id; }
    quint32 id() { return _id; }
    void setName(QString name) { _name = name; }
    QString name() { return _name; }

private:
    quint32 _id;
    QString _name;
};

#endif // FLYGROUP_H
