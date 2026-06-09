#include "WechatImporter.h"

WechatImporter::WechatImporter()
{

}
int WechatImporter::importFile(const QString &path)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "打开文件失败";

        return 0;
    }

    //--------------------------------------
    // 第一次：打印前20行调试
    //--------------------------------------
    {
        QTextStream debugStream(&file);

#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
        debugStream.setCodec("GB18030");
#endif

        qDebug() << "====================";

        for(int i = 0; i < 20 && !debugStream.atEnd(); i++)
        {
            qDebug() << i << ":" << debugStream.readLine();
        }

        qDebug() << "====================";
    }

    //--------------------------------------
    // 回到文件开头
    //--------------------------------------
    file.seek(0);

    //--------------------------------------
    // 第二次：真正解析
    //--------------------------------------
    QTextStream in(&file);

#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
    in.setCodec("GB18030");
#endif

    BillService service;

    int successCount = 0;
    int duplicateCount = 0;

    //--------------------------------------
    // 找表头
    //--------------------------------------
    bool foundHeader = false;

    while(!in.atEnd())
    {
        QString line = in.readLine();

        if(line.contains("交易时间"))
        {
            qDebug() << "找到微信账单表头";

            foundHeader = true;

            break;
        }
    }

    if(!foundHeader)
    {
        qDebug() << "没有找到微信账单表头";

        file.close();

        return 0;
    }

    //--------------------------------------
    // 开始导入
    //--------------------------------------
    while(!in.atEnd())
    {
        QString line = in.readLine();

        if(line.trimmed().isEmpty())
        {
            continue;
        }

        qDebug() << "账单：" << line;

        QStringList fields = line.split(",");

        qDebug() << "fields size =" << fields.size();

        //--------------------------------------
        // 微信正常是11列
        //--------------------------------------
        if(fields.size() < 11)
        {
            qDebug() << "字段不足，跳过";

            continue;
        }

        //--------------------------------------
        // 支付状态
        //--------------------------------------
        QString status = fields[7].trimmed();

        if(status != "支付成功")
        {
            qDebug() << "非支付成功";

            continue;
        }

        BillInfo bill;
        bill.transactionId =
                fields[8].trimmed();

        //--------------------------------------
        // 时间
        //--------------------------------------
        bill.createTime =
                fields[0].trimmed();

        bill.updateTime =
                bill.createTime;

        //--------------------------------------
        // 收支
        //--------------------------------------
        QString type =
                fields[4].trimmed();

        bill.type =
                (type == "支出")
                ? 0
                : 1;

        //--------------------------------------
        // 金额
        //--------------------------------------
        QString amount =
                fields[5];

        amount.remove("￥");
        amount.remove("¥");
        amount.remove(",");

        amount = amount.trimmed();

        bill.amount =
                amount.toDouble();

        //--------------------------------------
        // 商品
        //--------------------------------------
        QString goods =
                fields[3].trimmed();

        //--------------------------------------
        // 备注
        //--------------------------------------
        bill.remark =
                fields[2].trimmed();

        //--------------------------------------
        // 自动分类
        //--------------------------------------
        QString category = "其他";

        if(goods.contains("蒸旺")
                || goods.contains("蜜雪")
                || goods.contains("瑞幸")
                || goods.contains("麦当劳")
                || goods.contains("肯德基")
                || goods.contains("奶茶")
                || goods.contains("手工包"))
        {
            category = "餐饮";
        }
        else if(goods.contains("滴滴")
                || goods.contains("高德")
                || goods.contains("地铁"))
        {
            category = "交通";
        }
        else if(goods.contains("药房")
                || goods.contains("医院"))
        {
            category = "医疗";
        }
        else if(goods.contains("能耗费")
                || goods.contains("电费")
                || goods.contains("水费")
                || goods.contains("燃气"))
        {
            category = "住房";
        }
        else if(goods.contains("淘宝")
                || goods.contains("京东")
                || goods.contains("拼多多"))
        {
            category = "购物";
        }

        bill.category = category;

        //--------------------------------------
        // 入库
        //--------------------------------------
//        if(service.addBill(bill))
//        {
//            successCount++;

//            qDebug()
//                    << "导入成功："
//                    << bill.amount
//                    << bill.category;
//        }
//        else
//        {
//            qDebug()
//                    << "数据库插入失败";
//        }
        if(service.addBill(bill))
        {
            successCount++;

            qDebug()
                    << "导入成功："
                    << bill.transactionId;
        }
        else
        {
            duplicateCount++;

            qDebug()
                    << "重复账单："
                    << bill.transactionId;
        }
    }

    file.close();

//    qDebug()
//            << "导入完成，共导入"
//            << successCount
//            << "条";

//    return successCount;
    qDebug()
            << "导入完成："
            << "成功"
            << successCount
            << "条，重复"
            << duplicateCount
            << "条";

    return successCount;
}
