#include "datamanager.h"

QSqlDatabase *DataManager::notesDatabase = new QSqlDatabase();

DataManager::DataManager()
{
    // 建立一个QSqlDatabase对象，后续操作会使用这个对象
//    notesDatabase = new QSqlDatabase();
    if(QSqlDatabase::contains(DATABASE_CONNECTTIONNAME))                         // 判断指定的连接是否存在
    {
        (*notesDatabase) = QSqlDatabase::database(DATABASE_CONNECTTIONNAME);     // 如果指定的连接存在，则直接返回这个连接
    }
    else                                                                         // 如果指定的连接不存在，则创建连接，并添加数据库
    {
        (*notesDatabase) = QSqlDatabase::addDatabase("QSQLITE", DATABASE_CONNECTTIONNAME);    // 按照指定数据库类型和连接名称创建数据库
        notesDatabase->setDatabaseName(DATABASE_NAME);                                        // 设置数据库文件名，如果存在，后续的操作将在这个数据库上操作，如果不存在，则自动创建

        // 测试代码
//        if(notesDatabase->open())
//        {
//            QSqlQuery query(*notesDatabase);
//            query.exec("create table student (id int primary key, "
//                       "name varchar(20))");
//            query.exec("insert into student values(0, 'first')");
//            query.exec("insert into student values(1, 'second')");
//            query.exec("insert into student values(2, 'third')");
//            query.exec("insert into student values(3, 'fourth')");
//            query.exec("insert into student values(4, 'fifth')");
//        }
    }
}

DataManager::~DataManager()
{
    delete notesDatabase;
}

bool DataManager::openDatabase()
{
    if(!notesDatabase->open())
    {
        qWarning() << "Error: Failed to connect database." << notesDatabase->lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to connect database. %1").arg(notesDatabase->lastError().text()));
        return false;
    }

    return true;
}

void DataManager::closeDatabase()
{
    notesDatabase->close();
}

/**
 * @brief DataManager::isTableExist
 * 判断指定表是否存在
 * @param tableName
 * @return
 */
bool DataManager::isTableExist(const QString &tableName)
{
    QSqlQuery query(*notesDatabase);
    QString querySql = QString("select count(*) from sqlite_master where type = 'table' and name = '%1'").arg(tableName);
    query.prepare(querySql);
    return query.exec();
}

/**
 * @brief DataManager::createTable
 * 指定表名创建表
 * @param tableName
 * @return
 */
bool DataManager::createTable(const QString &tableName)
{
    // 创建一个表格
    // id(自增id作为主键)      note_title(待办项标题)      note_context(待办项内容)      note_time(设置待办的时间)      is_fin(表示待办项是否完成)
    QSqlQuery query(*notesDatabase);
    QString createSql = QString("create table %1 (id int primary key, note_title varchar(30), note_context varchar(150), note_time char(20), is_fin int)").arg(tableName);
    query.prepare(createSql);
    if(!query.exec())
    {
        qWarning() << "Error: Failed to create table." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to create table. %1").arg(query.lastError().text()));
        return false;
    }
    return true;
}

/**
 * @brief DataManager::insertData
 * 指定表名插入数据
 * @param tableNmae
 * @param note
 * @return
 */
bool DataManager::insertData(const QString &tableName, Notes &note)
{
    QSqlQuery query(*notesDatabase);
    QString insertSql = QString("insert into %1 values (?, ?, ?, ?, ?)").arg(tableName);
    query.prepare(insertSql);
//    query.addBindValue(query.lastInsertId().toInt() + 1);
    note.id = queryMaxID(tableName) + 1;
    query.addBindValue(note.id);
    query.addBindValue(note.noteTitle);
    query.addBindValue(note.noteContext);
    query.addBindValue(note.noteTime);
    query.addBindValue(0);

    if(!query.exec())
    {
        qWarning() << "Error: Failed to insert data." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to insert data. %1").arg(query.lastError().text()));
        return false;
    }
    return true;
}

bool DataManager::updateData(const QString &tableName, Notes &note, int id)
{
    QSqlQuery query(*notesDatabase);
    QString updataSql = QString("update %1 set note_title = ?, note_context = ?, note_time = ?, is_fin = ? where id = ?").arg(tableName);
//    QString updataSql = QString("updata %1 set note_title = %2, note_context = %3, note_time = %4, is_fin = %5 where id = %6")
//            .arg(tableName)
//            .arg(note.noteTitle)
//            .arg(note.noteContext)
//            .arg(note.noteTime)
//            .arg(note.isFin)
//            .arg(id);
    query.prepare(updataSql);
//    query.bindValue(":id", id);
//    query.bindValue(":title", note.noteTitle);
//    query.bindValue(":context", note.noteContext);
//    query.bindValue(":time", note.noteTime);
//    query.bindValue(":tag", note.isFin);
    query.addBindValue(note.noteTitle);
    query.addBindValue(note.noteContext);
    query.addBindValue(note.noteTime);
    query.addBindValue(note.isFin);
    query.addBindValue(id);
    if(!query.exec())
    {
        qWarning() << "Error: Failed to update data." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to update data. %1").arg(query.lastError().text()));
        return false;
    }
    return true;
}

bool DataManager::updateState(const QString &tableName, int id, int isDone)
{
    QSqlQuery query(*notesDatabase);
    QString updataSql = QString("update %1 set is_fin = ? where id = ?").arg(tableName);
    query.prepare(updataSql);
    query.addBindValue(isDone);
    query.addBindValue(id);
    if(!query.exec())
    {
        qWarning() << "Error: Failed to update state." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to update state. %1").arg(query.lastError().text()));
        return false;
    }
    return true;
}

bool DataManager::queryData(const QString &tableNmae, Notes &note, int id)
{
    QSqlQuery query(*notesDatabase);
    QString querySql = QString("select * from %1 where id = :id").arg(tableNmae);
    query.prepare(querySql);
    query.bindValue(":id", id);
    if(!query.exec())
    {
        qWarning() << "Error: Failed to query data." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to query data. %1").arg(query.lastError().text()));
        return false;
    }
    else
    {
        if(query.next())
        {
            note.id = query.value(0).toInt();
            note.noteTitle = query.value(1).toString();
            note.noteContext = query.value(2).toString();
            note.noteTime = query.value(3).toString();
            note.isFin = query.value(4).toInt();
        }
        return true;
    }
}

//bool DataManager::queryAllData(const QString &tableName, QVector<Notes *> &noteToBeDone, QVector<Notes *> &noteFinished)
bool DataManager::queryAllData(const QString &tableName, QVector<Notes *> &notes)
{
    QSqlQuery query(*notesDatabase);
    QString queryAllSql = QString("select * from %1").arg(tableName);
    query.prepare(queryAllSql);
    if(!query.exec())
    {
        qWarning() << "Error: Failed to query all data." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to query all data. %1").arg(query.lastError().text()));
        return false;
    }
    QString title, context, time;
    int id, tag;
    while(query.next())
    {
        id = query.value(0).toInt();
        title = query.value(1).toString();
        context = query.value(2).toString();
        time = query.value(3).toString();
        tag = query.value(4).toInt();
//        if(tag)
//        {
//            noteFinished.push_back(new Notes(id, title, context, time, tag));
//        }
//        else
//        {
//            noteToBeDone.push_back(new Notes(id, title, context, time, tag));
//        }
        notes.push_back(new Notes(id, title, context, time, tag));
    }
    return true;
}

int DataManager::queryMaxID(const QString &tableName)
{
    QSqlQuery query(*notesDatabase);
    QString queryMaxSql = QString("select max(id) from %1").arg(tableName);
    int maxID = 0;
    query.prepare(queryMaxSql);
    if(!query.exec())
    {
        qWarning() << "Error: Failed to query max id." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to query max id. %1").arg(query.lastError().text()));
    }
    else
    {
        while(query.next())
        {
            maxID = query.value(0).toInt();
        }
    }
    return maxID;
}

bool DataManager::deleteData(const QString &tableName, int id)
{
    QSqlQuery query(*notesDatabase);
    QString deleteSql = QString("delete from %1 where id = ?").arg(tableName);
    query.prepare(deleteSql);
    query.addBindValue(id);
    if(!query.exec())
    {
        qWarning() << "Error: Failed to delete data." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to delete data.").arg(query.lastError().text()));
        return false;
    }

    return true;
}

bool DataManager::deleteAllData(const QString &tableName)
{
    QSqlQuery query(*notesDatabase);
    QString deleteSql = QString("delete from %1").arg(tableName);
    query.prepare(deleteSql);
    if(!query.exec())
    {
        qWarning() << "Error: Failed to delete all data." << query.lastError();
        Log::getLogger()->LOG_ERROR(QString("Error: Failed to delete all data. %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}
















