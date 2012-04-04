#include "WaitWidget.h"
#include "ui_WaitWidget.h"

WaitWidget::WaitWidget(FlyCore *core, QWidget *parent) :
    QWidget(parent), ui(new Ui::WaitWidget), _core(core)
{
    ui->setupUi(this);
    connect(_core, SIGNAL(updateWorkStatus(fWork::WorkStatus)), this, SLOT(onUpdateWorkStatus(fWork::WorkStatus)));
    onUpdateWorkStatus(fWork::statusNone);
}

//void WaitWidget::closeEvent(QCloseEvent *event)
//{
//    disconnect(this, SLOT(onUpdateWorkStatus(fWork::WorkStatus)));
//    event->accept();
//}

void WaitWidget::onUpdateWorkStatus(fWork::WorkStatus status)
{
    switch (status) {
    case fWork::statusNone:
    {
        ui->lText->setText(tr("   Добро пожаловать в систему тестирования FlyWork!\nСейчас прослушайте вашего руководителя и ознакомьтесь с предстоящей работой. При возникновении вопросов относительно работы с данной программой, спросите вашего руководителя."));
        ui->lBusyIndicator->setText(tr("Загрузка информации о работе..."));
    }
        break;
    case fWork::statusPause:
    {
        ui->lText->setText(tr("   Добро пожаловать в систему тестирования FlyWork!\nСейчас прослушайте вашего руководителя и ознакомьтесь с предстоящей работой. При возникновении вопросов относительно работы с данной программой, спросите вашего руководителя."));
        ui->lBusyIndicator->setText(tr("Ожидание начала работы..."));
    }
        break;
    case fWork::statusStart:
    {
        ui->lText->setText(tr("   Добро пожаловать в систему тестирования FlyWork!\nСейчас прослушайте вашего руководителя и ознакомьтесь с предстоящей работой. При возникновении вопросов относительно работы с данной программой, спросите вашего руководителя."));
        ui->lBusyIndicator->setText(tr("Запуск рабочего окружения..."));
    }
        break;
    case fWork::statusStop:
    {
        ui->lText->setText(tr("   Добро пожаловать в систему тестирования FlyWork!\nСейчас прослушайте вашего руководителя и ознакомьтесь с предстоящей работой. При возникновении вопросов относительно работы с данной программой, спросите вашего руководителя."));
        ui->lBusyIndicator->setText(tr("Работа завершена..."));
    }
        break;
    }
}
