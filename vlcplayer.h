#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include "vlclib.h"
#include "getconf.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class VlcPlayer; }
QT_END_NAMESPACE

class VlcPlayer : public QWidget
{
    Q_OBJECT

public:
    VlcPlayer(QWidget *parent = nullptr);
    ~VlcPlayer();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_local_clicked();

    void on_stop_clicked();

    void on_playorpause_clicked();

    void on_mute_clicked();

    void on_fullscreen_clicked();

    void update();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::VlcPlayer *ui;
    VlcLib * player;
    GetConf * conf;
};

#endif // VLCPLAYER_H
