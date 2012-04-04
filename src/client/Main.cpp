/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include <QApplication>
#include <QTextCodec>
#include "FlyCore.h"

int main(int argc, char *argv[])
{
    QTextCodec* codec =  QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);

    QApplication a(argc, argv);

    FlyCore *core = new FlyCore();
    core->startSession();

    return a.exec();
}
