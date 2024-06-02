 #include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "blackj.h"
#include "mines.h"
#include "timeoutwindow.h"
#include <QJSEngine>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(double balance, double net, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , generalBalance(balance)
    , net(net)
    , networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    ui->label_2->setText(QString::number(generalBalance));

    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::handleNetworkReply);
    onCheckBalance();
    QPixmap quarPng(":/quarcodes/quarAdress.png");
    ui->label_4->setPixmap(quarPng);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCheckBalance()
{
    QNetworkRequest request(QUrl("http://localhost:3000/checkBalance"));
    networkManager->get(request);
}

void MainWindow::withdrawMoney(const QString &address, double amount)
{
    QUrl url("http://localhost:3000/withdrawMoney?address=" + address + "&amount=" + QString::number(amount));
    QNetworkRequest request(url);
    networkManager->get(request);
}

void MainWindow::handleNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
        double balance = jsonObj["balance"].toDouble();
        generalBalance = balance + net;
        ui->label_2->setText(QString::fromStdString(std::to_string(generalBalance)));

        //QMessageBox::information(this, "Balance", QString("Balance: %1 SOL").arg(balance));
    } else {
        QMessageBox::critical(this, "Error", reply->errorString());
    }
    reply->deleteLater();
}

void MainWindow::on_pushButton_clicked()
{
    generalBalance += 1000;
    ui->label_2->setText(QString::number(generalBalance));
}



void MainWindow::on_pushButton_2_clicked()
{
    hide();
    blackj *black = new blackj(generalBalance, net, this);
    black->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    hide();
    mines *field = new mines(generalBalance, net, this);
    field->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    onCheckBalance();
}


void MainWindow::on_pushButton_5_clicked()
{
    QString address = "ATTmEgXHXC6QCiJhvKM853rzvtZcBRS6RheaSUknsT5M";
    double amount = 0.001;
    //withdrawMoney(address, amount);
    hide();
    timeoutwindow *timeout = new timeoutwindow(generalBalance, net, this);
    timeout->show();

}

