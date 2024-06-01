#include "mainwindow.h"
#include "balanceManager.h"
#include "pokerwindow.h"
#include "./ui_mainwindow.h"
#include "loginization.h"
#include <QMovie>
#include <QUrl>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QCoreApplication>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)


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

    QMovie* noProfit = new QMovie(":/files/img/noProfit.gif");
    ui->gifLabel->setMovie(noProfit);
    noProfit->start();


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




    // player = new QMediaPlayer(this);
    // player->setSource(QUrl::fromLocalFile("/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/kudasai.mp3"));
    // audioOutput = new QAudioOutput(this);
    // player->setAudioOutput(audioOutput);
    // audioOutput->setVolume(50);
    // ui->songNameLabel->setText("Kudasai");
    // player->play();

    // ui->songVolSlider->setRange(0, 100);
    // ui->songVolSlider->setValue(50);
    // ui->songPosSlider->setRange(0, player->duration());

    // connect(ui->songVolSlider, &QSlider::valueChanged, this, [&](int value) {
    //     audioOutput->setVolume(value / 100.0);
    // });

    // connect(ui->songPosSlider, &QSlider::sliderMoved, this, [&](int position) {
    //     player->setPosition(position);
    // });

    // connect(player, &QMediaPlayer::positionChanged, this, [&](qint64 position) {
    //     ui->songPosSlider->setValue(position);
    // });

    // connect(player, &QMediaPlayer::durationChanged, this, [&](qint64 duration) {
    //     ui->songPosSlider->setRange(0, duration);
    // });


    connect(&BalanceManager::balanceInstance(), &BalanceManager::balanceChanged,
           this, &MainWindow::updateBalanceDisplay);  // подписка на сигнал об обновлении баланса






}

MainWindow::~MainWindow()
{
    if(window) {
        delete window;
    }
    delete ui;
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    SDL_Quit();
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




