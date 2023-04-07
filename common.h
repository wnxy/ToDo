#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDateTime>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QFile>
#include <QColor>

#include "log.h"

const QString DATABASE_CONNECTTIONNAME = QString("qt_sql_notes_connection");
const QString DATABASE_NAME = QString("NotesDatabase.db");
const QString DATABASE_TABLENAME = QString("NotesData");

extern volatile bool isToDoMove;              // 标志电子便签是否可移动
extern volatile bool autoStart;               // 标志程序是否设置开机自启动
extern QColor noteFontColor;                  // 待办事项字体颜色
extern QColor mainWinBGColor;                 // 主界面窗口背景颜色
extern QColor noteEditBGColor;                // 待办事项编辑区域背景颜色
extern QString fontFamily;                    // 字体
extern int fontPointSize;                     // 字号
extern int mainWinBGAlpha;                    // 主界面窗口背景透明度
extern int noteEditBGAlpha;                   // 待办事项编辑区域背景透明度

/**
 * @brief The Notes struct
 * 待办事项数据结构
 */
struct Notes
{
    int id;
    QString noteTitle;
    QString noteContext;
    QString noteTime;
    int isFin;               // 标志待办项是否完成，0：未完成   1：已完成
    Notes() : id(0), noteTitle(""), noteContext(""), noteTime(""), isFin(0) {}
    Notes(int id, QString &title, QString &context, QString &time, int tag)
        : id(id), noteTitle(title), noteContext(context), noteTime(time), isFin(tag) {}

    bool empty()
    {
        return id == 0;
    }

    void clear()
    {
        this->id = 0;
        this->noteTitle = "";
        this->noteContext = "";
        this->noteTime = "";
        this->isFin = 0;
    }

    void copy(const Notes &cp)
    {
        this->id = cp.id;
        this->noteTitle = cp.noteTitle;
        this->noteContext = cp.noteContext;
        this->noteTime = cp.noteTime;
        this->isFin = cp.isFin;
    }
};

/**
 * @brief The SortWeight struct
 * 自定义QVariant数据类型，待办事项进行排序的权重
 */
struct SortWeight
{
    int tag;
    QString time;
};
Q_DECLARE_METATYPE(SortWeight)

/**
 * @brief getCurrentDate
 * 获取当前时间，格式为 yyyy/MM/dd<delim>hh:mm:ss zzz
 * @param delim 分隔符
 * @return
 */
QString getCurrentDate(const char &delim);

/**
 * @brief compare
 * 比较两个时间的前后
 * @param str1
 * @param str2
 * @return
 */
bool compare(const QString &str1, const QString &str2);

/**
 * @brief alphaToOpacity
 * QColor中的alpha通道转换为透明度百分比opacity
 * alpha:0-255, 0表示完全透明 255表示完全不透明
 * opacity:0-100, 0表示完全不透明 100表示完全透明
 * @param a alpha通道
 * @return
 */
int alphaToOpacity(int a);

/**
 * @brief opacityToAlpha
 * 透明度百分比opacity转换为QColor中的alpha通道
 * @param opacity 透明度百分比
 * @return
 */
int opacityToAlpha(int opacity);

#endif // COMMON_H
