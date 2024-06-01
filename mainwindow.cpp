 #include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "blackj.h"
#include "mines.h"

MainWindow::MainWindow(int balance, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , generalBalance(balance)
{
    ui->setupUi(this);
    ui->label_2->setText(QString::number(generalBalance));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    generalBalance += 1000;
    ui->label_2->setText(QString::number(generalBalance));
}



void MainWindow::on_pushButton_2_clicked()
{
    hide();
    blackj *black = new blackj(generalBalance, this);
    black->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    hide();
    mines *field = new mines(generalBalance, this);
    field->show();
}

