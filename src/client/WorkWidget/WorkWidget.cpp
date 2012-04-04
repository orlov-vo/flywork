#include "WorkWidget.h"
#include "ui_WorkWidget.h"

#include <QtGui>

WorkWidget::WorkWidget(FlyCore *core, QWidget *parent) :
    QWidget(parent), ui(new Ui::WorkWidget), _core(core), _timer(new QTimer(this))
{
    ui->setupUi(this);

    connect(_core, SIGNAL(updateWork()), this, SLOT(onUpdateWork()));
    connect(_core, SIGNAL(updateTasks()), this, SLOT(onUpdateTasks()));
    connect(ui->lwTasks, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(onListTasksItemChanged(QListWidgetItem*,QListWidgetItem*)));
    connect(_timer, SIGNAL(timeout()), this, SLOT(doUpdateTime()));
}

//void WorkWidget::closeEvent(QCloseEvent *event)
//{
//    disconnect(this, SLOT(onListTasksItemChanged(QListWidgetItem*,QListWidgetItem*)));
//    disconnect(this, SLOT(doUpdateTime()));
//    event->accept();
//}

void WorkWidget::reloadWidget()
{
    ui->lUsername->setText(_core->user()->username());
    ui->lGroup->setText(_core->user()->groupName());

    _timer->start(1000);
}

void WorkWidget::doUpdateTime()
{
    QTime time;
    int secs = QTime::currentTime().secsTo(_core->work()->endTime());
    if (secs > 0) {
        ui->teTimer->setTime(time.addSecs(secs));
    } else {
        ui->teTimer->setTime(time.addSecs(0));
        _timer->stop();
    }
}

void WorkWidget::onListTasksItemChanged(QListWidgetItem *newitem, QListWidgetItem *olditem)
{
    if (newitem == NULL || newitem == olditem)
        return;
    ui->pbSend->setVisible(true);
    _currentTask = _core->searchTask(newitem->data(32).toUInt());
    if (_currentTask == NULL)
        return;

    ui->gbTask->setTitle(_currentTask->name());
    ui->tbContent->setText(_currentTask->content());

    _core->clearLayout(ui->formAnswers);

    if (_currentTask->type() == fTask::inputBox) {
        lAnswer = new QLabel(tr("Ваш ответ:"));
        leAnswer = new QLineEdit();
        leAnswer->setPlaceholderText(tr("Введите ваш ответ"));

        ui->formAnswers->insertRow(1, lAnswer, leAnswer);

    }
    if (_currentTask->type() == fTask::tempFile) {
        lAnswer = new QLabel(tr("Ваш файл:"));
        pbAnswer = new QPushButton();
        pbAnswer->setObjectName("pbAnswer");
        pbAnswer->setText(tr("Открыть ваш файл"));
        connect(pbAnswer, SIGNAL(clicked()), this, SLOT(on_pbAnswer_clicked()));

        ui->formAnswers->insertRow(1, lAnswer, pbAnswer);
    }
}

void WorkWidget::onUpdateWork()
{
    ui->lWorkTitle->setText(_core->work()->title());
}

void WorkWidget::onUpdateTasks()
{
    ui->lwTasks->clear();
    QList<FlyTask *> tasks = _core->tasks();
    for(int n = 0; n < tasks.count(); n++)
    {
        QListWidgetItem *item = new QListWidgetItem(tasks.at(n)->name());
        item->setData(32, tasks.at(n)->id());
        ui->lwTasks->addItem(item);
    }
}

void WorkWidget::on_pbSend_clicked()
{
    if (_currentTask->type() != fTask::inputBox)
        return;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint32)_currentTask->id();
    out << leAnswer->text();
    FlyNetwork::doSendPacket(_core->socket(), CMSG_TASK_SEND, block);
}

void WorkWidget::on_pbAnswer_clicked()
{
    qDebug() << "on_pbAnswer_clicked()";
}
