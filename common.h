#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDateTime>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QFile>

const QString DATABASE_CONNECTTIONNAME = QString("qt_sql_notes_connection");
const QString DATABASE_NAME = QString("NotesDatabase.db");
const QString DATABASE_TABLENAME = QString("NotesData");

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

#endif // COMMON_H
