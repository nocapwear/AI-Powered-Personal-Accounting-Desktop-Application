#include "DatabaseManager.h"
QSqlDatabase DatabaseManager::database()
{
    return m_db;
}
DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager db;

    return db;
}

bool DatabaseManager::init()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");

//    m_db.setDatabaseName("bill.db");
    // 先获取exe目录，向上跳转一层，再拼接bill.db
    QDir exeDir(QCoreApplication::applicationDirPath());
    exeDir.cdUp(); // 从debug → 跳到dll文件夹
    QString dbPath = exeDir.filePath("bill.db");
    m_db.setDatabaseName(dbPath);

    if(!m_db.open())
    {
        qDebug() << "open db failed";

        return false;
    }

    QSqlQuery query;

//    QString sql =
//            "CREATE TABLE IF NOT EXISTS bill ("
//            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//            "uuid TEXT,"
//            "amount REAL,"
//            "type INTEGER,"
//            "category TEXT,"
//            "remark TEXT,"
//            "create_time TEXT,"
//            "update_time TEXT"
//            ");";
    // Qt 最佳建表语句（账单表）
    QString sql =
        "CREATE TABLE IF NOT EXISTS bill ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"  // 自增主键
        "transaction_id TEXT UNIQUE,"            // 唯一标识（替代uuid，强制不重复）
        "amount REAL,"                           // 金额
        "type INTEGER,"                          // 类型 1收入/0支出
        "category TEXT,"                         // 分类
        "remark TEXT,"                           // 备注
        "create_time TEXT,"                      // 创建时间
        "update_time TEXT"                       // 更新时间
        ");";

    if(!query.exec(sql))
    {
        qDebug() << query.lastError();

        return false;
    }

    query.exec(
        "CREATE UNIQUE INDEX IF NOT EXISTS "
        "idx_transaction_id "
        "ON bill(transaction_id)");

    qDebug() << "database init success";

    return true;
}
