#include "vlcplayer.h"
#include "ui_vlcplayer.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <QStringListModel>
#include <QThread>

#include <QDebug>

VlcPlayer::VlcPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VlcPlayer)
{
    ui->setupUi(this);
    player = new VlcLib;
//    conf = new GetConf("channels.conf");
//    QStringListModel * model = new QStringListModel(conf->get_names());
//    ui->listView->setModel(model);
    ui->widget->installEventFilter(this);
    ui->horizontalSlider->installEventFilter(this);
    ui->horizontalSlider_2->installEventFilter(this);

    QTimer * timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &VlcPlayer::update);
    timer->start(100);

    on_playorpause_clicked();
    on_stop_clicked();
}

VlcPlayer::~VlcPlayer()
{
    delete ui;
}

bool VlcPlayer::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        if (watched == ui->widget)
        {
            QKeyEvent * keyset = (QKeyEvent *)event;
            if (keyset->key() == Qt::Key_Escape)
            {
                if (ui->widget->isFullScreen())
                {
                    this->show();
                    ui->widget->setWindowFlags(Qt::SubWindow);
                    ui->widget->showNormal();
                    return true;
                }
            }
            else if (keyset->key() == Qt::Key_Return)
            {
                if (ui->widget->isFullScreen())
                {
                    this->show();
                    ui->widget->setWindowFlags(Qt::SubWindow);
                    ui->widget->showNormal();
                    return true;
                }
                else
                {
                    ui->widget->setWindowFlags(Qt::Window);
                    ui->widget->showFullScreen();
                    this->hide();
                }
            }
        }
    }
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (watched == ui->horizontalSlider)
        {
            QMouseEvent * mouset = (QMouseEvent *)event;
            if (mouset->button() == Qt::LeftButton)
            {
                double pro = (double)mouset->x() / ui->horizontalSlider->width();
                double pos = pro * ui->horizontalSlider->maximum();
                ui->horizontalSlider->setValue(pos);
                player->setPosition(pro);
                return true;
            }
        }
        else if (watched == ui->horizontalSlider_2)
        {
            QMouseEvent * mouset = (QMouseEvent *)event;
            if (mouset->button() == Qt::LeftButton)
            {
                double pro = (double)mouset->x() / ui->horizontalSlider_2->width();
                double pos = pro * ui->horizontalSlider_2->maximum();
                ui->horizontalSlider_2->setValue(pos);
                player->setVolume(pro * 101);
                return true;
            }
        }
    }
    return false;
}

void VlcPlayer::on_local_clicked()
{
//    QString filename = QFileDialog::getOpenFileName(this, tr("Video or Channel"), "", tr("Video File (*.mp4 *.avi *.wav);;Conf File (*.conf);; Txt File (*.txt)"));
//    if (filename.endsWith("conf") || filename.endsWith("txt"))
//    {
//        conf = new GetConf(filename);
//        QStringListModel * model = new QStringListModel(conf->get_names());
//        ui->listView->setModel(model);
//        return ;
//    }
//    filename.replace("/", "\\");
    QString filename1 = QString("http://vfx.mtime.cn/Video/2019/02/04/mp4/190204084208765161.mp4");
    QString filename = QString("rtsp://152.136.165.215:554/openUrl/0dF07AY?beginTime=20220408T000000&endTime=20220408T000030&playBackMode=1");
    player->playFile(filename1, (HWND)ui->widget->winId());
    ui->horizontalSlider->show();
}

void VlcPlayer::on_playorpause_clicked()
{
    if (player == nullptr)
    {
        return ;
    }
    player->play();
}

void VlcPlayer::on_stop_clicked()
{
    if (player == nullptr)
    {
        return ;
    }
    player->stop();
}

void VlcPlayer::on_mute_clicked()
{
    if (player == nullptr)
    {
        return ;
    }
    player->mute();
}

void VlcPlayer::on_fullscreen_clicked()
{
    if (player == nullptr)
    {
        return ;
    }

    ui->widget->setWindowFlags(Qt::Window);
    ui->widget->showFullScreen();
    this->hide();
}

void VlcPlayer::update()
{
    QTime n(0, 0, 0), cur, tol;
    float position = player->getPosition();
    int volume = player->getVolume();
    libvlc_time_t curtime = player->getTime();
    libvlc_time_t toltime = player->getLength();
    cur = n.fromMSecsSinceStartOfDay(curtime);
    tol = n.fromMSecsSinceStartOfDay(toltime);
    if (toltime > 3600000)
    {
    ui->lineEdit->setText(cur.toString("hh:mm:ss"));
    ui->lineEdit_2->setText(tol.toString("hh:mm:ss"));
    }
    else if (toltime > 60000)
    {
        ui->lineEdit->setText(cur.toString("mm:ss"));
        ui->lineEdit_2->setText(tol.toString("mm:ss"));
    }
    else if (toltime > 1000)
    {
        ui->lineEdit->setText(cur.toString("ss"));
        ui->lineEdit_2->setText(tol.toString("ss"));
    }
    else if (toltime > 0)
    {
        ui->lineEdit->setText(cur.toString("zzz"));
        ui->lineEdit_2->setText(tol.toString("zzz"));
    }
    ui->horizontalSlider->setValue(position * ui->horizontalSlider->maximum());
    ui->lineEdit_3->setText(QString::number(volume) + "%");
    ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->maximum() * volume / 100.0);
    if (player->isEnd())
    {
        player->stop();
        ui->horizontalSlider->setValue(0);
        ui->lineEdit->clear();
    }
}

void VlcPlayer::on_listView_clicked(const QModelIndex &index)
{
    player->stop();
    QString filename = conf->get_urls().at(index.row());
    if (filename.isEmpty())
    {
        return ;
    }
    if (filename.endsWith("\n"))
    {
        filename = filename.split("\n")[0];
    }
    player->playFile(filename, (HWND)ui->widget->winId());
}
