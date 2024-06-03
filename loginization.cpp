#include "loginization.h"
#include "ui_loginization.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QTabWidget>
#include <QGraphicsOpacityEffect>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>


// loginization::loginization(QWidget *parent)
//     : QDialog(parent)
//     , ui(new Ui::loginization)
// {
//     ui->setupUi(this);
    // ui->usernameSignupBar->setPlaceholderText("koshei");
    // ui->loginSignupBar->setPlaceholderText("azino777");
    // ui->passwordSignupBar->setPlaceholderText("B228OP");
    // ui->passwordRepeatSignupBar->setPlaceholderText("A228MP");
// }

// loginization::~loginization()
// {
//     delete ui;
// }

// void loginization::on_signInLoginButton_clicked()
// {
//     QString loginIn = ui->loginSigninBar->text();
//     QString passwordIn = ui->passwordSigninBar->text();

//     //ищем по бдшке сходства
//     if (loginIn == "МС Пох" && passwordIn == "весенний лес") {
//         QMessageBox::information(this, "", "Хайпуешь, плесень");
//     } else {
//         QMessageBox::warning(this, "", "Что ты сделал для хипхопа в свои годы?");
//     }
// }




// void loginization::on_signupLoginButton_clicked()
// {
//     QString loginUp = ui->loginSignupBar->text();
//     QString passwordUp = ui->passwordSignupBar->text();
//     QMessageBox::information(this, "", "ищи себя в прошмандовках Таджикистана");
//     //добавляем в бдшку
// }




loginization::loginization(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::loginization)
{
    ui->setupUi(this);
    ui->usernameSignupBar->setPlaceholderText("koshei");
    ui->loginSignupBar->setPlaceholderText("azino777");
    ui->passwordSignupBar->setPlaceholderText("B228OP");
    ui->passwordRepeatSignupBar->setPlaceholderText("A228MP");
}

loginization::~loginization()
{
    delete ui;
}

void loginization::on_radioButton_clicked(bool checked)
{
    if (checked){
        QMessageBox::warning(this, "", "Аахахахахаха ну ты лошара я ору пон");
    }

}

void loginization::on_signupLoginButton_clicked()
{
    sqlite3* db;
    char* errMsg = 0;
    int rc = sqlite3_open("/Users/capybastercarbonaster/Desktop/RegistrationDB.db", &db);

    std::string sqlStatement = "CREATE TABLE IF NOT EXISTS data ("
                               "username TEXT, "
                               "login TEXT, "
                               "password TEXT, "
                               "salt TEXT)";
    // std::string sqlStatement = "CREATE TABLE IF NOT EXISTS data ("
    //                            "username TEXT PRIMARY KEY, "
    //                            "login TEXT, "
    //                            "password TEXT, "
    //                            "salt TEXT)";
    rc = sqlite3_exec(db, sqlStatement.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error";
        sqlite3_free(errMsg);
    }
    QSqlDatabase dbq = QSqlDatabase::addDatabase("QSQLITE");
    dbq.setDatabaseName("/Users/capybastercarbonaster/Desktop/RegistrationDB.db");
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


    srand(time(0));

    const std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int n = letters.size();
    QString randomString;

    for (int i = 0; i < 10; ++i) {
        randomString.push_back(letters[rand() % n]);
    }

    QString username = ui->usernameSignupBar->text();
    QString password = ui->passwordSignupBar->text();
    QString login  = ui->loginSignupBar->text();
    QString repeatPassword = ui->passwordRepeatSignupBar->text();
    std::hash<QString> hasher;
    size_t hashedPassword = hasher(password+randomString);
    QString NormalHashedPassword = QString::number(hashedPassword);
    std::cout << username.toStdString()<<'\t';
    std::cout << login.toStdString()<<'\n';
    std::cout<<hasher(password)<<'\n';
    std::cout<<randomString.toStdString()<<'\n';

    auto pointer = std::find(logins.begin(), logins.end(), login);



    if(username==""){
        std::cout<<"no username"<<'\n';
        QMessageBox::information(this, "имя пользователя не введено","куда ж нам без имени пользователя...");
    }else if(login==""){
        std::cout<<"no login"<<'\n';
        QMessageBox::information(this, "логин не введен!","логин временно отсутствует... может потом вернется?");
    }else if(password==""){
        std::cout<<"no password"<<'\n';
        QMessageBox::information(this, "пароль не введен!","а пароль где? ручки-то, вот они!");
    }else if(pointer!=logins.end()){
        std::cout<<"such login has been already created"<<'\n';
        QMessageBox::information(this, "такой логин уже существует","прояви фантазию, придумай уникальный логин!");
    }else if(password!=repeatPassword){
        std::cout<<"passwords are not the same"<<'\n';
        QMessageBox::information(this, "пароли не совпадают","мошенник! пароли не совпали!");
    }else{
        sqlStatement = "INSERT INTO data (username, login, password, salt) VALUES ('" + username.toStdString() + "', '" + login.toStdString() + "', '" + NormalHashedPassword.toStdString() + "','" + randomString.toStdString() + "');";
        rc = sqlite3_exec(db, sqlStatement.c_str(), 0, 0, &errMsg);
        sqlite3_close(db);
        std::cout<<"registered successfully"<<'\n';
        QMessageBox::information(this, "","ищи себя в прошмандовках Таджикистана");
    }
}



void loginization::on_signInLoginButton_clicked()
{
    QSqlDatabase dbq = QSqlDatabase::addDatabase("QSQLITE");
    dbq.setDatabaseName("/Users/capybastercarbonaster/Desktop/RegistrationDB.db");
    if (!dbq.open()) {
        std::cout << "Unable to open the database"; // без этого не работает
        QMessageBox::information(this, "бд не подключена","чушпанчик, базу данных кто подлючать будет? не по пацански...");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT username, login, password, salt FROM data");
    if (!query.exec()) {
        std::cout << "Unable to make the correct query";
        QMessageBox::information(this, "запрос не выполнен","снова ошибочка...");
        return;
    }
    std::vector<QString> usernames;
    std::vector<QString> logins;
    std::vector<QString> passwords;
    std::vector<QString> salts;
    while (query.next()) {
        auto username = query.value("username").toString();
        auto login = query.value("login").toString();
        auto password = query.value("password").toString();
        auto salt = query.value("salt").toString();
        usernames.push_back(username);
        logins.push_back(login);
        passwords.push_back(password);
        salts.push_back(salt);
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
    for (QString x:salts){
        std::cout<<x.toStdString()<<'\t';
    }
    std::cout<<'\n';

    QString insertedLogin = ui->loginSigninBar->text();

    QString insertedPassword = ui->passwordSigninBar->text();
    std::hash<QString> hasher;
    std::cout << insertedLogin.toStdString()<<'\t';
    std::cout << insertedPassword.toStdString()<<'\n';
    int index;
    bool flag=false;
    auto point = std::find(logins.begin(), logins.end(), insertedLogin);
    if (point!=logins.end()){
        index=point-logins.begin();
        flag=true;
    }
    else{
        std::cout<<"no such login"<<'\n';
        QMessageBox::information(this, "такого логина не существует","такого логина не существует, попробуйте вспомнить свой логин");
    }
    //std::cout<<index<<'\n';
    size_t insertedHashedPassword = hasher(insertedPassword+salts[index]);
    QString NormalInsertedHashedPassword = QString::number(insertedHashedPassword);
    QString userName = usernames[index];
    //std::cout<<username.toString();
    if (flag && NormalInsertedHashedPassword==passwords[index]){
        std::cout<<"you are allowed to visit our casino"<<'\n';
        QMessageBox::information(this, "победа!","проходи, будь как дома");
        hide();
        MainWindow *hub = new MainWindow(this, insertedLogin, userName);
        hub->show();

    }else if(flag && NormalInsertedHashedPassword!=passwords[index]){
        std::cout<<"wrong password"<<'\n';
        QMessageBox::information(this, "неверный пароль","чушпанам здесь не рады!! крути педали, пока не дали");
    }
}









