#include "timeoutwindow.h"
#include "ui_timeoutwindow.h"
#include "mainwindow.h"


timeoutwindow::timeoutwindow(double balance, double net, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::timeoutwindow)
    , curBalance(balance)
    , net(net)
    , timer(new QTimer(this))
    , updateTimer(new QTimer(this))
    , timeLabel(new QLabel(this))
    , remainingTime(180000)
{
    ui->setupUi(this);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setText("3:00");
    ui->verticalLayout->addWidget(timeLabel);
    connect(timer, &QTimer::timeout, this, &timeoutwindow::onTimeout);
    connect(updateTimer, &QTimer::timeout, this, &timeoutwindow::updateRemainingTime);
}


timeoutwindow::~timeoutwindow()
{
    delete ui;
}

void timeoutwindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    timer->setSingleShot(true); // Ensures the timer only fires once
    timer->start(remainingTime);

    updateTimer->start(1000);
}


void timeoutwindow::onTimeout()
{
    hide();
    MainWindow *hub = new MainWindow(curBalance, net, this);
    hub->show();
}



void timeoutwindow::updateRemainingTime()
{
    remainingTime -= 1000;

    int minutes = (remainingTime / 1000) / 60;
    int seconds = (remainingTime / 1000) % 60;

    timeLabel->setText(QString("%1:%2")
                           .arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0')));

    if (remainingTime <= 0)
    {
        updateTimer->stop();
    }
}

