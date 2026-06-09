#ifndef BILLSERVICE_H
#define BILLSERVICE_H
#include "DataBase/BillInfo.h"
#include "Database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QUuid>
#include <QList>


class BillService
{
public:

    bool addBill(const BillInfo& bill);
    bool deleteBill(int id);

    QList<BillInfo> getAllBills();
    bool updateBill(const BillInfo& bill);
    QList<BillInfo> searchBills(const QString& amount,int type,const QString& category,const QString& remark);
    QMap<QString,double>getExpenseStatistics();

    double getTotalIncome();
    double getTotalExpense();
    QStringList getTopExpenseCategories();
    QList<QPair<QString,double>>getExpenseRanking();
};

#endif
