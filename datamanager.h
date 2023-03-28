#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#include "common.h"

class DataManager
{
public:
    DataManager();
    ~DataManager();

    bool openDatabase();                                                               // 打开指定数据库
    void closeDatabase();                                                              // 关闭指定数据库
    bool isTableExist(const QString &tableName);                                       // 判断指定表是否存在
    bool createTable(const QString &tableName);                                        // 指定表名创建表
    bool insertData(const QString &tableName, Notes &note);                            // 指定表名，插入一条数据
    bool updateData(const QString &tableName, Notes &note, int id);                    // 更新数据内容
    bool updateState(const QString &tableName, int id, int isDone);                    // 更新数状态
    bool queryData(const QString &tableName, Notes &note, int id);                     // 查询数据
//    bool queryAllData(const QString &tableName, QVector<Notes *>&, QVector<Notes *>&);
    bool queryAllData(const QString &tableName, QVector<Notes *>&);                    // 查询所有数据
//    bool deleteData(const QString &tableName);
    int queryMaxID(const QString &tableName);                                          // 查询最大ID
    bool deleteData(const QString &tableName, int id);
    bool deleteAllData(const QString &tableName);

private:
    QSqlDatabase *notesDatabase;         // 被操作的数据库对象
};

#endif // DATAMANAGER_H
