#pragma once // 头文件防重复包含
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager
{
public:

    static DatabaseManager& instance();

    bool init();
    QSqlDatabase database();

private:
    DatabaseManager(){}

private:

    QSqlDatabase m_db;
};

