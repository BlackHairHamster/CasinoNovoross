#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlDriver>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->username->setPlaceholderText("koshei");
    ui->loginReg->setPlaceholderText("azino777");
    ui->passwordReg->setPlaceholderText("B228OP");
    ui->repeatPassword->setPlaceholderText("A228MP");
    //connect(ui->regButton, &QPushButton::clicked, this, on_regButton_clicked());
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_regButton_clicked()
{
    // database = QSqlDatabase::addDatabase ("QMYSQL");
    // database.setHostName ("localhost");
    // database.setPort(3306);
    // database.setUserName ("root");
    // database.setPassword("Man200508");
    // database.setDatabaseName("RegistrationAndLogin");

    // if (!database.open()) {
    //     QString text = database.lastError().text();
    // }
    // else
    //     qDebug() << "С ДБ все норм";

    // // qDebug() <<
    // if(database.open()){

    //     QString username = ui->username->text();
    //     QString password = ui->passwordReg->text();
    //     QString login  = ui->loginReg->text();

    //     QSqlQuery qry;
    //     qry.prepare("INSERT INTO users (username, login, password)"
    //                 "VALUES (:username, :login, :password)");
    //     qry.bindValue(":username", username);
    //     qry.bindValue(":login", login);
    //     qry.bindValue(":password", password);
    //     if (qry.exec()){
    //         QMessageBox::information(this, "inserted", "Success");
    //     }else{
    //         QMessageBox::information(this, "not inserted", "Fail");
    //     }
    //     }else{
    //     QMessageBox::information(this, "бд не подключена","чушпанчик, базу данных кто подлючать будет? не по пацански...");

    // }



    sqlite3* db;
    char* errMsg = 0;
    int rc = sqlite3_open("/Users/mike_napolov/Documents/RegistrationDB.db", &db);

    std::string sqlStatement = "CREATE TABLE IF NOT EXISTS data ("
                               "username TEXT, "
                               "login TEXT, "
                               "password TEXT)";
    rc = sqlite3_exec(db, sqlStatement.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error";
        sqlite3_free(errMsg);
    }
    QSqlDatabase dbq = QSqlDatabase::addDatabase("QSQLITE");
    dbq.setDatabaseName("/Users/mike_napolov/Documents/RegistrationDB.db");
    if (!dbq.open()) {
        std::cout << "Unable to open the database"; // без этого не работает
        QMessageBox::information(this, "бд не подключена","чушпанчик, базу данных кто подлючать будет? не по пацански...");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT username, login, password FROM data");
    if (!query.exec()) {
        std::cout << "Unable to make the correct query"<<'\n';
        QMessageBox::information(this, "запрос не выполнен","снова ошибочка с бдшкой...");
        return;
    }
    std::vector<QString> logins;
    while (query.next()) {
        auto login = query.value("login").toString();
        logins.push_back(login);
    }


    //int user_id = 1;
    QString username = ui->username->text();
    QString password = ui->passwordReg->text();
    QString login  = ui->loginReg->text();
    QString repeatPassword = ui->repeatPassword->text();
    std::cout << username.toStdString()<<'\t';
    std::cout << login.toStdString()<<'\n';
    std::hash<QString> hasher;
    size_t hashedPassword = hasher(password);
    QString NormalHashedPassword = QString::number(hashedPassword);
    std::cout<<hasher(password)<<'\n';

    auto pointer = std::find(logins.begin(), logins.end(), login);
    if (password!=repeatPassword){
        std::cout<<"passwords are not the same"<<'\n';
        QMessageBox::information(this, "пароли не совпадают","мошенник! пароли не совпали!");
    }else if(pointer!=logins.end()){
        std::cout<<"such login has been already created"<<'\n';
        QMessageBox::information(this, "такой логин уже существует","прояви фантазию, придумай уникальный логин!");
    }else{

        sqlStatement = "INSERT INTO data (username, login, password) VALUES ('" + username.toStdString() + "', '" + login.toStdString() + "', '" + NormalHashedPassword.toStdString() + "');";
        rc = sqlite3_exec(db, sqlStatement.c_str(), 0, 0, &errMsg);
        sqlite3_close(db);
    }
}



void MainWindow::on_logButton_clicked()
{
    QSqlDatabase dbq = QSqlDatabase::addDatabase("QSQLITE");
    dbq.setDatabaseName("/Users/mike_napolov/Documents/RegistrationDB.db");
    if (!dbq.open()) {
        std::cout << "Unable to open the database"; // без этого не работает
        QMessageBox::information(this, "бд не подключена","чушпанчик, базу данных кто подлючать будет? не по пацански...");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT username, login, password FROM data");
    if (!query.exec()) {
        std::cout << "Unable to make the correct query";
        QMessageBox::information(this, "запрос не выполнен","снова ошибочка...");
        return;
    }
    std::vector<QString> usernames;
    std::vector<QString> logins;
    std::vector<QString> passwords;
    while (query.next()) {
        auto username = query.value("username").toString();
        auto login = query.value("login").toString();
        auto password = query.value("password").toString();
        usernames.push_back(username);
        logins.push_back(login);
        passwords.push_back(password);
    }
    for (QString x:usernames){
        std::cout<<x.toStdString()<<'\t';

    }
    std::cout<<'\n';
    for (QString x:logins){
        std::cout<<x.toStdString()<<'\t';

    }
    std::cout<<'\n';
    for (QString x:passwords){
        std::cout<<x.toStdString()<<'\t';

    }
    std::cout<<'\n';
    QString insertedLogin = ui->login->text();
    QString insertedPassword = ui->password->text();
    std::cout << insertedLogin.toStdString()<<'\t';
    std::cout << insertedPassword.toStdString()<<'\n';
    int index;
    auto point = std::find(logins.begin(), logins.end(), insertedLogin);
    if (point!=logins.end()){
        index=point-logins.begin();
    }
    else{
        std::cout<<"no such login"<<'\n';
        QMessageBox::information(this, "такого логина не существует","такого логина не существует, попробуйте вспомнить свой логин");
    }
    std::cout<<index<<'\n';
    if (insertedPassword==passwords[index]){
        std::cout<<"you are allowed to visit our casino"<<'\n';
        QMessageBox::information(this, "победа!","проходи, будь как дома");
    }else{
        std::cout<<"wrong password"<<'\n';
        QMessageBox::information(this, "неверный пароль","чушпанам здесь не рады!! крути педали, пока не дали");
    }
}

