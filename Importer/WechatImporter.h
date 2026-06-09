#ifndef WECHATIMPORTER_H
#define WECHATIMPORTER_H

#include <QString>
#include "Database/BillInfo.h"
#include "Service/BillService.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

class WechatImporter
{
public:
    WechatImporter();
    int importFile(const QString& path);
};

#endif
