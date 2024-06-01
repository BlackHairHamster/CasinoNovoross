#include "musicplayer.h"
#include <QFileInfo>
#include <QString>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent), player(new QMediaPlayer(this)), audioOutput(new QAudioOutput(this)), currentTrackIndex{0} {
    player->setAudioOutput(audioOutput);

    connect(player, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        emit positionChanged(static_cast<int>(position));
    });

    connect(player, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        emit durationChanged(static_cast<int>(duration));
    });

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            next();

        }
    });



}


void MusicPlayer::play() {
    player->play();
}

void MusicPlayer::pause() {
    player->pause();
}


void MusicPlayer::setVolume(int volume) {
    audioOutput->setVolume(static_cast<float>(volume) / 100.0f);
}

void MusicPlayer::setPosition(int position) {
    player->setPosition(static_cast<qint64>(position));
}

QString extractFileName(const QString& path){
    QFileInfo fileInfo(path);
    return fileInfo.baseName();
}

QString MusicPlayer::getCurrentTrackName(const QStringList &playlist) const {

    return extractFileName(playlist.at(currentTrackIndex));
}

void MusicPlayer::setAudioSource(const QString &source) {
    currentTrack = source;
    player->setSource(QUrl::fromLocalFile(source));
}

void MusicPlayer::setPlaylist(const QStringList &playlist) {
    this->playlist = playlist;
    if (!playlist.isEmpty()) {
        currentTrackIndex = 0;
        setAudioSource(playlist.first());
    }
}

void MusicPlayer::next() {
    if (playlist.isEmpty()) return;
    if (++currentTrackIndex >= playlist.size()) {
        currentTrackIndex = 0; // Вернуться к началу плейлиста
    }
    playCurrentIndex();
}

void MusicPlayer::prev() {
    if (playlist.isEmpty()) return;
    if (--currentTrackIndex < 0) {
        currentTrackIndex = playlist.size() - 1;
    }

    playCurrentIndex();
}


void MusicPlayer::playCurrentIndex() {
    if (currentTrackIndex < 0 || currentTrackIndex >= playlist.size()) return;
    setAudioSource(playlist.at(currentTrackIndex));
    play();
}

