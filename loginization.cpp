#include "loginization.h"
#include "ui_loginization.h"
#include <QMessageBox>

loginization::loginization(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginization)
{
    ui->setupUi(this);
}

loginization::~loginization()
{
    delete ui;
}

void loginization::on_signInLoginButton_clicked()
{
    QString loginIn = ui->loginSigninBar->text();
    QString passwordIn = ui->passwordSigninBar->text();
    //ищем по бдшке сходства
    if (loginIn == "МС Пох" && passwordIn == "весенний лес") {
        QMessageBox::information(this, "", "Хайпуешь, плесень");
    } else {
        QMessageBox::warning(this, "", "Что ты сделал для хипхопа в свои годы?");
    }
}

void loginization::on_radioButton_clicked(bool checked)
{
    if (checked){
        QMessageBox::warning(this, "", "Аахахахахаха ну ты лошара я ору пон");
    }

}


void loginization::on_signupLoginButton_clicked()
{
    QString loginUp = ui->loginSignupBar->text();
    QString passwordUp = ui->passwordSignupBar->text();
    QMessageBox::information(this, "", "ищи себя в прошмандовках Таджикистана");
    //добавляем в бдшку
}



