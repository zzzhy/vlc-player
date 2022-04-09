#ifndef GETCONF_H
#define GETCONF_H

#include <QWidget>
#include <QFile>

class GetConf : public QFile
{
    Q_OBJECT
public:
    GetConf(QString filename);

    QStringList get_names();
    QStringList get_urls();

private:
    QStringList names;
    QStringList urls;
};

#endif // GETCONF_H
