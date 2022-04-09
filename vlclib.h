#ifndef VLCLIB_H
#define VLCLIB_H

#include <vlc/vlc.h>
#include <QWidget>
#include <QQueue>

class VlcLib : public QWidget
{
    Q_OBJECT
public:
    explicit VlcLib(QWidget *parent = nullptr);

public slots:
    void playFile(QString filename, HWND hwnd);

    void play();
    void stop();

    void mute();

    libvlc_time_t getLength();
    libvlc_time_t getTime();

    float getPosition();
    int getVolume();

    void setPosition(float pos);
    void setVolume(int vol);

    bool isEnd();

signals:

private:
    libvlc_instance_t * vlc_inst = nullptr;
    libvlc_media_t * vlc_media = nullptr;
    libvlc_media_player_t * vlc_player = nullptr;
    QQueue<float> recentPositions;
};

#endif // VLCLIB_H
