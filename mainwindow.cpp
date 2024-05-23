#include "mainwindow.h"
#include "balanceManager.h"
#include "pokerwindow.h"
#include "./ui_mainwindow.h"
#include "loginization.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->balanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
    QIcon posterButtonIcon1(QPixmap(":/files/img/poster1.png"));
    ui->posterButton1->setIcon(posterButtonIcon1);
    ui->posterButton1->setIconSize(ui->posterButton1->size());
    QIcon posterButtonIcon2(QPixmap(":/files/img/poster2.png"));
    ui->posterButton2->setIcon(posterButtonIcon2);
    ui->posterButton2->setIconSize(ui->posterButton2->size());
    QIcon posterButtonIcon3(QPixmap(":/files/img/poster3.png"));
    ui->posterButton3->setIcon(posterButtonIcon3);
    ui->posterButton3->setIconSize(ui->posterButton3->size());
    connect(&BalanceManager::balanceInstance(), &BalanceManager::balanceChanged,
           this, &MainWindow::updateBalanceDisplay);  // подписка на сигнал об обновлении баланса


}

MainWindow::~MainWindow()
{
    if(window) {
        delete window;
    }
    delete ui;
}


void MainWindow::on_depositButton_clicked()
{
    BalanceManager::balanceInstance().depositBalance(1000);
    ui->balanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
}


void MainWindow::on_posterButton1_clicked()
{
    if(!window) {
        window = new pokerwindow(this);
        connect(window, &pokerwindow::balanceChanged, this, &MainWindow::updateBalanceDisplay);
    }

    window->show();
}

void MainWindow::updateBalanceDisplay(int newBalance)
{
    ui->balanceNum->setText(QString::number(newBalance));
}


void MainWindow::on_signInButton_clicked()
{
    loginWindow = new loginization(this);
    loginWindow->show();

}


void MainWindow::on_signUpButton_clicked()
{
    loginWindow = new loginization(this);
    loginWindow->show();
}

