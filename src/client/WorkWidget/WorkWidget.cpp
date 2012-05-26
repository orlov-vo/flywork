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

void WorkWidget::reloadWidget()
{
    ui->lUsername->setText(_core->user()->username());
    ui->lGroup->setText(_core->user()->groupName());

    ui->gbTask->setTitle(tr("Система тестирования FlyWork"));
    ui->tbContent->setHtml(
        tr("<h2>Добро пожаловать!</h2>") +
        tr("Приветствуем Вас в интерактивной системе тестирования! Работа уже началась, однако мы можем кратко помочь Вам разобраться с программой. Для этого прочтите данный текст.") +
        tr("<ul>") +
        tr("<li>Справа, в контекстном меню, вы можете перемещаться по заданиям.</li>") +
        tr("<li>Задания бывают различных типов: с выбором правильного ответа, работой с файлом</li>") +
        tr("<li>Все внесенные ответы, а также файлы сразу же сохраняются.</li>") +
        tr("<li>Изменять ответы можно сколько угодно, главное успеть внести изменения до окончания работы.</li>") +
        tr("</ul>") +
        tr("<b>Желаем Вам успехов!</b>"));

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

    _currentTask = _core->searchTask(newitem->data(32).toUInt());
    if (_currentTask == NULL)
        return;

    ui->gbTask->setTitle(_currentTask->name());
    ui->tbContent->setHtml(_currentTask->content());

    _core->clearLayout(ui->formAnswers);

    if (_currentTask->type() == fTask::inputBox) {
        lAnswer = new QLabel(tr("Ваш ответ:"));
        leAnswer = new QLineEdit();
        leAnswer->setPlaceholderText(tr("Введите ваш ответ"));

        FlyAnswer *found_answer = _core->searchAnswer(_currentTask->id());
        if (found_answer != NULL)
            leAnswer->setText(found_answer->answer());

        ui->formAnswers->insertRow(1, lAnswer, leAnswer);
        connect(leAnswer, SIGNAL(editingFinished()), this, SLOT(onAnswerDone()));
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

void WorkWidget::onAnswerDone()
{
    if (_currentTask->type() == fTask::inputBox) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint32)_currentTask->id();
        out << leAnswer->text();
        FlyAnswer *temp = new FlyAnswer(_currentTask->id(), leAnswer->text());
        _core->addAnswer(temp);
        FlyNetwork::doSendPacket(_core->socket(), CMSG_TASK_SEND, block);
    }
}

void WorkWidget::on_pbAnswer_clicked()
{
    qDebug() << "on_pbAnswer_clicked()";
}
