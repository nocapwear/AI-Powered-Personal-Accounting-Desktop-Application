#include "BillService.h"

bool BillService::addBill(const BillInfo &bill)
{
    QSqlQuery query(DatabaseManager::instance().database());

    QString sql =
        "INSERT OR IGNORE INTO bill "
        "(transaction_id,"
        " amount,"
        " type,"
        " category,"
        " remark,"
        " create_time,"
        " update_time) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)";

    query.prepare(sql);

    query.addBindValue(bill.transactionId);
    query.addBindValue(bill.amount);
    query.addBindValue(bill.type);
    query.addBindValue(bill.category);
    query.addBindValue(bill.remark);
    query.addBindValue(bill.createTime);
    query.addBindValue(bill.updateTime);

    if(!query.exec())
    {
        qDebug()
                << "insert failed:"
                << query.lastError().text();

        return false;
    }

    return query.numRowsAffected() > 0;
}
bool BillService::deleteBill(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare("DELETE FROM bill WHERE id = ?");

    query.addBindValue(id);

    if(!query.exec())
    {
        qDebug()
                << "delete failed:"
                << query.lastError().text();

        return false;
    }

    return true;
}
bool BillService::updateBill(const BillInfo& bill)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "UPDATE bill "
        "SET amount=?, "
        "type=?, "
        "category=?, "
        "remark=?, "
        "update_time=? "
        "WHERE id=?");

    query.addBindValue(bill.amount);

    query.addBindValue(bill.type);

    query.addBindValue(bill.category);

    query.addBindValue(bill.remark);

    query.addBindValue(bill.updateTime);

    query.addBindValue(bill.id);

    if(!query.exec())
    {
        qDebug()
                << "update failed:"
                << query.lastError().text();

        return false;
    }

    return true;
}
QList<BillInfo> BillService::searchBills(const QString& amount,int type,const QString& category,const QString& remark)
{
    QList<BillInfo> bills;

    QSqlQuery query(DatabaseManager::instance().database());

    QString sql =
            "SELECT id,"
            "amount,"
            "type,"
            "category,"
            "remark,"
            "create_time "
            "FROM bill "
            "WHERE 1=1 ";

    if(!amount.isEmpty())
    {
        sql += "AND amount=? ";
    }

    if(type!=-1)
    {
        sql += "AND type=? ";
    }

    if(category!="请选择")
    {
        sql += "AND category=? ";
    }

    if(!remark.isEmpty())
    {
        sql += "AND remark LIKE ? ";
    }

    sql += "ORDER BY id DESC";

    query.prepare(sql);

    if(!amount.isEmpty())
    {
        query.addBindValue(amount.toDouble());
    }

    if(type!=-1)
    {
        query.addBindValue(type);
    }

    if(category!="请选择")
    {
        query.addBindValue(category);
    }

    if(!remark.isEmpty())
    {
        query.addBindValue(
                    "%" + remark + "%");
    }

    if(!query.exec())
    {
        qDebug()
                << query.lastError();

        return bills;
    }

    while(query.next())
    {
        BillInfo bill;

        bill.id =
                query.value(0).toInt();

        bill.amount =
                query.value(1).toDouble();

        bill.type =
                query.value(2).toInt();

        bill.category =
                query.value(3).toString();

        bill.remark =
                query.value(4).toString();

        bill.createTime =
                query.value(5).toString();

        bills.append(bill);
    }

    return bills;
}

QMap<QString,double> BillService::getExpenseStatistics()
{
    QMap<QString,double> result;
    QSqlQuery query(DatabaseManager::instance().database());

    // 修复1：使用 ABS() 取金额绝对值，保证饼图数值为正
    // 修复2：给SUM字段加别名，代码更规范、不易出错
    query.prepare(R"(
        SELECT category,
               SUM(ABS(amount)) AS total_amount  -- 取绝对值求和
        FROM bill
        WHERE type = 0  -- type=0代表支出，保持不变
        GROUP BY category
    )");

    if(!query.exec())
    {
        qDebug() << "查询支出统计失败：" << query.lastError();
        return result;
    }

    // 修复3：使用字段名取值，比索引更安全
    while(query.next())
    {
        QString category = query.value("category").toString();
        double total = query.value("total_amount").toDouble();

        // 过滤金额为0的无效分类
        if(total > 0) {
            result.insert(category, total);
        }
    }

    return result;
}
double BillService::getTotalIncome()
{
    QSqlQuery query(
                DatabaseManager::instance()
                .database());

    query.prepare(
                "SELECT IFNULL(SUM(amount),0) "
                "FROM bill "
                "WHERE type=1");

    if(query.exec() && query.next())
    {
        return query.value(0).toDouble();
    }

    return 0;
}
double BillService::getTotalExpense()
{
    QSqlQuery query(
                DatabaseManager::instance()
                .database());

    query.prepare(
                "SELECT IFNULL(SUM(amount),0) "
                "FROM bill "
                "WHERE type=0");

    if(query.exec() && query.next())
    {
        return query.value(0).toDouble();
    }

    return 0;
}
QStringList BillService::getTopExpenseCategories()
{
    QStringList result;
    double totalExpense = getTotalExpense();
    QSqlQuery query(
                DatabaseManager::instance()
                .database());

    query.prepare(
                "SELECT category,"
                "SUM(amount) "
                "FROM bill "
                "WHERE type=0 "
                "GROUP BY category "
                "ORDER BY SUM(amount) DESC ");

    if(!query.exec())
    {
        qDebug()
                << query.lastError();

        return result;
    }

    int rank = 1;

    while(query.next())
    {
        QString category =
                query.value(0).toString();

        double amount =
                query.value(1).toDouble();
        double percent = 0.0;

        if(totalExpense > 0)
        {
          percent =amount/ totalExpense* 100.0;
        }

        QString medal;

        switch(rank)
        {
        case 1:
            medal = "🥇";
            break;
        case 2:
            medal = "🥈";
            break;
        case 3:
            medal = "🥉";
            break;
        default:
//            medal = QString("[%1️]").arg(rank);
            break;
        }
        result.append(
                QString("%1 %2. %3：%4 元 (%5%)")
                    .arg(medal)
                    .arg(rank)
                    .arg(category)
                    .arg(amount,0,'f',2)
                    .arg(percent,0,'f',1));

        rank++;
    }

    return result;
}
QList<QPair<QString,double>>
BillService::getExpenseRanking()
{
    QList<QPair<QString,double>> result;

    QSqlQuery query(
        DatabaseManager::instance()
        .database());

    QString sql =
        "SELECT category,"
        "SUM(amount) "
        "FROM bill "
        "WHERE type=0 "
        "GROUP BY category "
        "ORDER BY SUM(amount) DESC";

    if(!query.exec(sql))
    {
        qDebug()
            << query.lastError().text();

        return result;
    }

    while(query.next())
    {
        QString category =
            query.value(0).toString();

        double amount =
            query.value(1).toDouble();

        result.append(
            qMakePair(
                category,
                amount));
    }

    return result;
}
