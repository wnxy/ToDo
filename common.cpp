#include "common.h"

volatile bool isToDoMove = true;                  // 标志电子便签是否可移动
volatile bool autoStart = true;                   // 标志程序是否设置开机自启动
QColor noteFontColor = Qt::black;                 // 待办事项字体颜色
QColor mainWinBGColor = QColor(97, 111, 118);     // 主界面窗口背景颜色
QColor noteEditBGColor = QColor(200, 207, 210);   // 待办事项编辑区域背景颜色
QString fontFamily = "Microsoft YaHei UI";        // 字体
int fontPointSize = 10;                           // 字号
int mainWinBGAlpha = 100;                         // 主界面窗口背景透明度
int noteEditBGAlpha = 255;                        // 待办事项编辑区域背景透明度


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

int alphaToOpacity(int a)
{
    int s = 255 - a;
    double dg = 0.39;
    int opacity = static_cast<int>(s * dg);
    if(opacity > 100) opacity = 100;
    if(opacity < 0) opacity = 0;
    return opacity;
}

int opacityToAlpha(int opacity)
{
    double dg = 2.55;
    int alpha = 255 - static_cast<int>(opacity * dg);
    if(alpha > 255) alpha = 255;
    if(alpha < 0) alpha = 0;
    return alpha;
}
