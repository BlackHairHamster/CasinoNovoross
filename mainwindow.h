#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "balanceManager.h"
#include "pokerwindow.h"
#include "musicplayer.h"
#include "loginization.h"
#include "pokerwindowhistory.h"
#include "mines.h"
#include "blackj.h"
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
    MainWindow(QWidget *parent = nullptr, QString login = "chuspan", QString userName = "chuspan");
    bool volIsUp = true;
    bool isPlaying = true;
    QStringList myPlaylist = {"/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/kudasai.mp3", "/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/Эщ Эщ.mp3", "/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/SVO.mp3", "/Users/capybastercarbonaster/Desktop/cpp/CasinoNovoross/files/music/Сучка.mp3"};
    ~MainWindow();

public slots:
    void updateBalanceDisplay(int newBalance);


private slots:
    void on_depositButton_clicked();
    void on_posterButton1_clicked();


    void on_songNextButton_clicked();

    void on_songPlayButton_clicked();

    void on_songVolButton_clicked();

    void on_songPrevButton_clicked();

    void on_posterButton2_clicked();

    void on_posterButton3_clicked();

    void on_logOutButton_clicked();

    void on_pokerHistoryButton_clicked();

private:
    Ui::MainWindow *ui;
    pokerwindow *window;
    blackj *blackjwindow;
    mines *mineswindow;
    MusicPlayer *musicPlayer;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QString login;
    QString userName;
    pokerwindowhistory* historyWindow;



};
#endif // MAINWINDOW_H
