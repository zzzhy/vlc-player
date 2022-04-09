#include "vlclib.h"
#include <QFile>
#include <QMessageBox>

VlcLib::VlcLib(QWidget *parent) : QWidget(parent)
{
    /* Load the VLC engine */
       const char* const vlc_args[] = {
//           "--demux=h265",
           "--ipv4",
           "--no-prefer-system-codecs",
           "--rtsp-caching=300",
           "--network-caching=500",	//网络额外缓存值 (ms)
           "--rtsp-frame-buffer-size=10000000",
           "--rtsp-tcp",				//RTSP采用TCP传输方式
       };
    vlc_player = nullptr;
    vlc_inst = libvlc_new(0, 0);
//    vlc_inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
}

void VlcLib::playFile(QString filename, HWND hwnd)
{
    if (filename.contains("rtmp") || filename.contains("rtsp") || filename.contains("http") || filename.contains("https"))
    {
        vlc_media = libvlc_media_new_location(vlc_inst, filename.toStdString().data());
    }
    else if (!QFile::exists(filename))
    {
        QMessageBox::warning(this, "player", "local file not found");
        return ;
    }
    else if (QFile::exists(filename))
    {
        vlc_media = libvlc_media_new_path(vlc_inst, filename.toStdString().data());
    }
    if (vlc_player && libvlc_media_player_is_playing(vlc_player))
    {
        libvlc_media_player_stop(vlc_player);
        libvlc_media_player_release(vlc_player);
        vlc_player = nullptr;
    }
    if (vlc_media)
    {
        vlc_player = libvlc_media_player_new_from_media(vlc_media);
    }
    if (vlc_player)
    {
        libvlc_media_release(vlc_media);
        if (hwnd)
        {
            libvlc_media_player_set_hwnd(vlc_player, hwnd);
        }
        libvlc_media_player_play(vlc_player);
    }
}

void VlcLib::play()
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    if (libvlc_media_player_is_playing(vlc_player))
    {
        libvlc_media_player_pause(vlc_player);
    }
    else
    {
        libvlc_media_player_play(vlc_player);
    }
}

void VlcLib::stop()
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    libvlc_state_t state = libvlc_media_player_get_state(vlc_player);
    switch (state) {
    case libvlc_Paused:
    case libvlc_Playing:
    case libvlc_Ended:
        libvlc_media_player_stop(vlc_player);
        break;
    default:
        break;
    }
}

void VlcLib::mute()
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    libvlc_audio_toggle_mute(vlc_player);
}

libvlc_time_t VlcLib::getLength()
{
    if (vlc_player == nullptr)
    {
        return 0;
    }
    return libvlc_media_player_get_length(vlc_player);
}

libvlc_time_t VlcLib::getTime()
{
    if (vlc_player == nullptr)
    {
        return 0;
    }
    return libvlc_media_player_get_time(vlc_player);
}

float VlcLib::getPosition()
{
    if (vlc_player == nullptr)
    {
        return 0;
    }
    return libvlc_media_player_get_position(vlc_player);
}

int VlcLib::getVolume()
{
    if (vlc_player == nullptr)
    {
        return 0;
    }
    return libvlc_audio_get_volume(vlc_player);
}

void VlcLib::setPosition(float pos)
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    libvlc_media_player_set_position(vlc_player, pos);
}

void VlcLib::setVolume(int vol)
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    libvlc_audio_set_volume(vlc_player, vol);
}

bool VlcLib::isEnd()
{
    if (vlc_player == nullptr)
    {
        return false;
    }
    libvlc_state_t state = libvlc_media_get_state(vlc_media);
    float pos = getPosition();
    if (state == libvlc_Playing && pos > 0) {
        if (recentPositions.count() <= 10) {
            recentPositions.enqueue(pos);
        } else {
            float *head = recentPositions.data();
            float h = *head, sum;
            int count = 0;
            for (sum = 0; count < recentPositions.count(); head ++) {
                sum += *head;
                count++;
            }
            if (sum == recentPositions.count() * h) {
                return true;
            }
            recentPositions.dequeue();
        }
        recentPositions.enqueue(pos);
    }
    return state == libvlc_Ended;
}
