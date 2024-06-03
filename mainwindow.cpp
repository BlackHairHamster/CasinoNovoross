#include "mainwindow.h"
#include "balanceManager.h"
#include "pokerwindow.h"
#include "pokerwindowhistory.h"
#include "mines.h"
#include "./ui_mainwindow.h"
#include "loginization.h"
#include <QMovie>
#include <QUrl>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QCoreApplication>
#include <iostream>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <sqlite3.h>




MainWindow::MainWindow(QWidget *parent, QString login, QString userName)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , login(login)
    , userName(userName)


{


    musicPlayer = new MusicPlayer(this);
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
    QPixmap koshei0(":/files/img/koshei0.png");
    ui->profilePhotoWidget->setPixmap(koshei0);
    ui->posterButton3->setIconSize(ui->posterButton3->size());

    ui->label->setText(userName);



    QPixmap nextIcon(":/files/img/songNextIcon.png");
    ui->songNextButton->setIcon(nextIcon);
    QPixmap pauseIcon(":/files/img/songPauseIcon.png");
    ui->songPlayButton->setIcon(pauseIcon);
    QPixmap prevIcon(":/files/img/songPrevIcon.png");
    ui->songPrevButton->setIcon(prevIcon);
    QPixmap volUpIcon(":/files/img/songVolUpIcon.png");
    ui->songVolButton->setIcon(volUpIcon);

    musicPlayer->setVolume(50);
    ui->songVolSlider->setValue(50);

    connect(ui->songVolSlider, &QSlider::valueChanged, musicPlayer, &MusicPlayer::setVolume);

    connect(ui->songPosSlider, &QSlider::sliderMoved, musicPlayer, &MusicPlayer::setPosition);

    connect(musicPlayer, &MusicPlayer::positionChanged, ui->songPosSlider, &QSlider::setValue);
    connect(musicPlayer, &MusicPlayer::durationChanged, this, [this](int duration) {
        ui->songPosSlider->setMaximum(duration);
    });

    musicPlayer->setPlaylist(myPlaylist);
    ui->songNameLabel->setText(musicPlayer->getCurrentTrackName(myPlaylist));
    musicPlayer->play();


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

void MainWindow::closeEvent(QCloseEvent *event) {
    std::string penis = std::to_string(BalanceManager::balanceInstance().getBalance());

    sqlite3* db;
    char* errMsg = 0;


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
    int index;
    auto point = std::find(logins.begin(), logins.end(), login);
    if (point!=logins.end()){
        index=point-logins.begin();
    }
    std::cout<<penis;
    std::string sqlStatement = "UPDATE data SET balance = '" + penis + "' WHERE login = '" + login.toStdString() + "' ;";
    // std::string sqlStatement = "SELECT * from data";

    const char* str1 = sqlStatement.c_str();
    std::cout<<penis<<'\t'<<sqlStatement;
    sqlite3_open("/Users/capybastercarbonaster/Desktop/RegistrationDB.db", &db);
    int rc = sqlite3_exec(db, str1, 0, 0, &errMsg);
    sqlite3_close(db);
}


void MainWindow::on_depositButton_clicked()
{
    BalanceManager::balanceInstance().depositBalance(1000);
    ui->balanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));

}


void MainWindow::on_posterButton1_clicked()
{
    if(!window) {
        window = new pokerwindow(this, login, userName);
        connect(window, &pokerwindow::balanceChanged, this, &MainWindow::updateBalanceDisplay);
    }
    window->show();
}

void MainWindow::on_posterButton2_clicked()
{
    if(!blackjwindow) {
        blackjwindow = new blackj(this, login, userName);
        connect(blackjwindow, &blackj::balanceChanged, this, &MainWindow::updateBalanceDisplay);
    }
    blackjwindow->show();
}

void MainWindow::on_posterButton3_clicked()
{
    if(!mineswindow) {
        mineswindow = new mines(this, login, userName);
        // connect(blackjwindow, &blackj::balanceChanged, this, &MainWindow::updateBalanceDisplay);
    }
    mineswindow->show();
}



void MainWindow::updateBalanceDisplay(int newBalance)
{
    ui->balanceNum->setText(QString::number(newBalance));
    QPixmap koshei0(":/files/img/koshei0.png");
    QPixmap koshei10(":/files/img/koshei10.png");
    QPixmap koshei20(":/files/img/koshey20.jpg");
    QPixmap koshei30(":/files/img/koshei30.png");
    QPixmap koshei40(":/files/img/koshei40.png");
    QPixmap koshei50(":/files/img/koshei50.jpg");
    if (newBalance >= 50000) ui->profilePhotoWidget->setPixmap(koshei50);
    else if (newBalance >= 40000) ui->profilePhotoWidget->setPixmap(koshei40);
    else if (newBalance >= 30000) ui->profilePhotoWidget->setPixmap(koshei30);
    else if (newBalance >= 20000) ui->profilePhotoWidget->setPixmap(koshei20);
    else if (newBalance >= 10000) ui->profilePhotoWidget->setPixmap(koshei10);
    else ui->profilePhotoWidget->setPixmap(koshei0);
}


void MainWindow::on_songNextButton_clicked()
{
    musicPlayer->next();
    ui->songNameLabel->setText(musicPlayer->getCurrentTrackName(myPlaylist));
}

void MainWindow::on_songPrevButton_clicked()
{
    musicPlayer->prev();
    ui->songNameLabel->setText(musicPlayer->getCurrentTrackName(myPlaylist));
}


void MainWindow::on_songPlayButton_clicked()
{
    if (isPlaying) {
        QPixmap playIcon(":/files/img/songPlayIcon.png");
        ui->songPlayButton->setIcon(playIcon);
        musicPlayer->pause();
        isPlaying = false;
    } else {
        QPixmap pauseIcon(":/files/img/songPauseIcon.png");
        ui->songPlayButton->setIcon(pauseIcon);
        musicPlayer->play();
        isPlaying = true;
    }
}


void MainWindow::on_songVolButton_clicked()
{
    if (volIsUp == true) {
        QPixmap muteIcon(":/files/img/songMuteIcon.png");
        ui->songVolButton->setIcon(muteIcon);
        ui->songVolSlider->setValue(0);
        volIsUp = false;
    } else {
        QPixmap volUpIcon(":/files/img/songVolUpIcon.png");
        ui->songVolButton->setIcon(volUpIcon);
        ui->songVolSlider->setValue(50);
        volIsUp = true;
    }
}




void MainWindow::on_logOutButton_clicked()
{
    hide();
    loginization* loginWindow = new loginization(this);
    loginWindow->show();
    QPixmap playIcon(":/files/img/songPlayIcon.png");
    ui->songPlayButton->setIcon(playIcon);
    musicPlayer->pause();
    isPlaying = false;

    std::string penis = std::to_string(BalanceManager::balanceInstance().getBalance());

    sqlite3* db;
    char* errMsg = 0;


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
    int index;
    auto point = std::find(logins.begin(), logins.end(), login);
    if (point!=logins.end()){
        index=point-logins.begin();
    }
    std::cout<<penis;
    std::string sqlStatement = "UPDATE data SET balance = '" + penis + "' WHERE login = '" + login.toStdString() + "' ;";
    // std::string sqlStatement = "SELECT * from data";

    const char* str1 = sqlStatement.c_str();
    std::cout<<penis<<'\t'<<sqlStatement;
    sqlite3_open("/Users/capybastercarbonaster/Desktop/RegistrationDB.db", &db);
    int rc = sqlite3_exec(db, str1, 0, 0, &errMsg);
    sqlite3_close(db);

}


void MainWindow::on_pokerHistoryButton_clicked()
{
    if(!historyWindow) {
        historyWindow = new pokerwindowhistory(this, userName, login);
    }
    historyWindow->show();

}



