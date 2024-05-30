#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QList>
#include <QUrl>

class MusicPlayer {
public:
    MusicPlayer() {
        connect(&player, &QMediaPlayer::mediaStatusChanged, this, &MusicPlayer::onMediaStatusChanged);
    }

    void setPlaylist(const QList<QUrl>& urls) {
        playlist = urls;
        currentIndex = -1;
    }

    void nextTrack() {
        if (playlist.isEmpty()) return;
        currentIndex = (currentIndex + 1) % playlist.size();
        player.setSource(playlist.at(currentIndex));
        player.play();
    }

    void previousTrack() {
        if (playlist.isEmpty()) return;
        currentIndex = (currentIndex - 1 + playlist.size()) % playlist.size(); // Чтобы избежать отрицательного индекса
        player.setSource(playlist.at(currentIndex));
        player.play();
    }

private:
    QMediaPlayer player;
    QList<QUrl> playlist;
    int currentIndex = -1;

    void onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            next(); // Автоматически переключиться на следующий трек после окончания текущего
        }
    }
};



#endif // MUSICPLAYER_H
