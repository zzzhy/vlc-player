#include "getconf.h"

GetConf::GetConf(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (5 <= strlen(line))
        {
            QString temp = QString(line);
            QStringList templist = temp.split(",");
            names.append(templist[0]);
            urls.append(templist[1]);
        }
    }
}

QStringList GetConf::get_names()
{
    return names;
}

QStringList GetConf::get_urls()
{
    return urls;
}
