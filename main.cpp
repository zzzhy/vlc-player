#include "vlcplayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VlcPlayer w;
//    w.setFixedSize(750, 500);
    w.setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding));
    w.setWindowFlags(Qt::Window|Qt::WindowCloseButtonHint);
    w.show();
    return a.exec();
}
