/*
 * Copyright (C) 2012 TesJin Group <http://www.tesjin.ru/>
 */

#include <QtCore/QCoreApplication>
#include <QTextCodec>
#include <QTime>

#include "FlyServer.h"
#include "revision.h"

void logOutput(QtMsgType type, const char *msg)
{
    QTime time = QTime::currentTime();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "[%s][Debug] %s\n", time.toString(Qt::TextDate).toAscii().data(), msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "[%s][Warning] %s\n", time.toString(Qt::TextDate).toAscii().data(), msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "[%s][Critical] %s\n", time.toString(Qt::TextDate).toAscii().data(), msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "[%s][Fatal] %s\n", time.toString(Qt::TextDate).toAscii().data(), msg);
        abort();
    }
}

int main(int argc, char *argv[])
{
    // Передаем аргументы в ядро Qt Framework
    QCoreApplication a(argc, argv);

    // Устанавливаем кодировку для всех наших текстов
    QTextCodec* codec =  QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);

    // Устанавливаем свой лог-обработчик
    qInstallMsgHandler(logOutput);

    // Выводим информацию в консоль о нашем приложении
    printf("FlyWork Server:   %s     (%s)\n", _HASH, _DATE);
    printf("Press <Ctrl-C> to stop this daemon.\n");
    printf("===============================================================\n");
    printf("  FFFFFFFF FFF       FFFFF           FFFFF             FF      \n");
    printf("  FFFFFFF   FFF   FFF FFF             FFF              FF      \n");
    printf("  FFF     FF FFF FFF  FFF             FFF              FF      \n");
    printf("  FFF     FFF FFFFF    FFF           FFF  FFFF  FFF FF FF  FF  \n");
    printf("  FFFFFF  FFF  FFF     FFF    FFF    FFF FFFFFF FFFFFF FF FF   \n");
    printf("  FFFFFFF FFF  FFF      FFF  FFFFF  FFF  FF  FF FF     FFFF    \n");
    printf("  FFF     FFF  FFF      FFFFFFFFFFFFFFF  FF  FF FF     FFFF    \n");
    printf("  FFF     FFF            FFFFF   FFFFF   FFFFFF FF     FF FF   \n");
    printf("  FFF     FFFFFFF         FFF     FFF     FFFF  FF     FF  FF  \n");
    printf("          FFFFFFFFF        FF      FF                       FF \n");
    printf("===============================================================\n");
    printf("FlyWork is free software; you can redistribute it and/or modify\n");
    printf("it under the terms of the GNU GPL v3.  Source code is  located:\n");
    printf(" - https://github.com/biziwalker/flywork                       \n");
    printf("===============================================================\n\n");

    // Создаем сервер
    FlyServer *cServer = new FlyServer();

    // По умолчанию запускаем сервер на 127.0.0.1:6425
    //TODO: добавить возможность изменять адрес
    cServer->doStartServer(QHostAddress::LocalHost, 6425);

    // Запускаем само приложение
    return a.exec();
}
