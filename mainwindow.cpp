#include "mainwindow.h"
#include "balanceManager.h"
#include "pokerwindow.h"
#include "./ui_mainwindow.h"
#include "loginization.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <QMovie>
#include <QUrl>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , backgroundMusic(nullptr)
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
    QPixmap koshei0(":/files/img/koshei0.png");
    ui->profilePhotoWidget->setPixmap(koshei0);
    ui->posterButton3->setIconSize(ui->posterButton3->size());

    QMovie* noProfit = new QMovie(":/files/img/noProfit.gif");
    ui->gifLabel->setMovie(noProfit);
    noProfit->start();



    connect(&BalanceManager::balanceInstance(), &BalanceManager::balanceChanged,
           this, &MainWindow::updateBalanceDisplay);  // подписка на сигнал об обновлении баланса

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %sn", SDL_GetError());
        // Обработка ошибки и выход из программы
    } else {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %sn", Mix_GetError());
            // Обработка ошибки и выход из программы
        } else {
            // Загружаем и воспроизводим музыку
            backgroundMusic = Mix_LoadMUS("/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/kudasai.mp3");
            if (backgroundMusic == nullptr) {
                printf("Failed to load background music! SDL_mixer Error: %sn", Mix_GetError());
            } else {
                Mix_PlayMusic(backgroundMusic, -1);
                Mix_VolumeMusic(MIX_MAX_VOLUME / 2); // Устанавливаем громкость (0 - 128)
            }
        }
    }






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

