#ifndef FMZIPINFO_H
#define FMZIPINFO_H

#include <QMap>
#include <QFile>

#include <quazip5/quazipfile.h>

class FMZipInfo
{
public:
    FMZipInfo();
    FMZipInfo(QString);
    QString &operator[](QString i);
    QMap<QString, QString> data;
};

#endif // FMZIPINFO_H
