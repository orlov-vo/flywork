/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include "FlyAnswer.h"

FlyAnswer::FlyAnswer(quint32 id, QString answer, QObject *parent) :
    QObject(parent), _id(id), _answer(answer)
{ }

void FlyAnswer::setId(quint32 id)
{
     _id = id;
}

void FlyAnswer::setAnswer(QString answer)
{
     _answer = answer;
     emit answerChanged(answer);
}
