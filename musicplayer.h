#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class MusicPlayer : public QObject
{
    Q_OBJECT

public:
    explicit MusicPlayer(QObject *parent = nullptr);
    void play();
    void pause();
    void setVolume(int volume);
    void setPosition(int position);
    QString getCurrentTrackName(const QStringList &playlist) const;
    void setAudioSource(const QString &source);
    void next();
    void prev();
    void setPlaylist(const QStringList &playlist);



signals:
    void positionChanged(int position);
    void durationChanged(int duration);

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QString currentTrack;
    QStringList playlist;
    int currentTrackIndex;
    void playCurrentIndex();

};

#endif // MUSICPLAYER_H

