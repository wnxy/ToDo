#include "common.h"

QString getCurrentDate(const char &delim)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString curDate = dateTime.toString("yyyy/MM/dd");
    QString curTime = dateTime.toString("hh:mm:ss");
//    QString curTime = dateTime.toString("hh:mm");
    QString curSec = dateTime.toString("zzz");
    return curTime + delim + curDate;
}

bool compare(const QString &str1, const QString &str2)
{
    QDateTime t1 = QDateTime::fromString(str1, "hh:mm:ss yyyy/MM/dd");
    QDateTime t2 = QDateTime::fromString(str2, "hh:mm:ss yyyy/MM/dd");
    return t1 < t2;
}
