#pragma once
#include <QString>

//struct BillInfo
//{
//    BillInfo(){}
//    int id = 0;

//    QString uuid;

//    double amount = 0;

//    int type = 0;

//    QString category;

//    QString remark;

//    QString createTime;

//    QString updateTime;
//};

struct BillInfo
{
    int id;

    QString transactionId;   // 新增

    double amount;

    int type;

    QString category;

    QString remark;

    QString createTime;

    QString updateTime;
};
