#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "balanceManager.h"
#include "pokerwindow.h"
#include "musicplayer.h"
#include "loginization.h"
#include "mines.h"
#include "blackj.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <QMediaPlayer>
#include <QAudioOutput>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool volIsUp = true;
    bool isPlaying = true;
    QStringList myPlaylist = {"/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/kudasai.mp3", "/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/Эщ Эщ.mp3", "/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/SVO.mp3", "/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/Сучка.mp3"};
    ~MainWindow();

public slots:
    void updateBalanceDisplay(int newBalance);


private slots:
    void on_depositButton_clicked();
    void on_posterButton1_clicked();
    void on_signInButton_clicked();
    void on_signUpButton_clicked();

    void on_songNextButton_clicked();

    void on_songPlayButton_clicked();

    void on_songVolButton_clicked();

    void on_songPrevButton_clicked();

    void on_posterButton2_clicked();

    void on_posterButton3_clicked();

private:
    Ui::MainWindow *ui;
    pokerwindow *window;
    loginization *loginWindow;
    blackj *blackjwindow;
    mines *mineswindow;
    Mix_Music *backgroundMusic;
    MusicPlayer *musicPlayer;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;

};
#endif // MAINWINDOW_H
