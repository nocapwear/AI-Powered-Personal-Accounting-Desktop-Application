#-------------------------------------------------
#
# Project created by QtCreator 2026-06-05T15:44:18
#
#-------------------------------------------------

# 核心编码配置 - 解决MSVC中文乱码/C2018编译错误
# 告诉Qt工具链源码使用UTF-8编码
CONFIG += utf8_source
# 仅对MSVC编译器生效，强制使用UTF-8解析源码和生成执行文件
win32-msvc*{
    QMAKE_CXXFLAGS += /utf-8
    QMAKE_CFLAGS += /utf-8
}

#QT       += core gui
#QT += sql
QT += core gui sql charts
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AIBillAgent
TEMPLATE = app

# 警告设置：使用已弃用Qt特性时发出警告
DEFINES += QT_DEPRECATED_WARNINGS

# 可选：取消注释以下行可禁用Qt 6.0.0之前的所有已弃用API
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# 使用C++11标准
CONFIG += c++11
# 源文件（带路径）
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Database/BillInfo.cpp \
    Database/DatabaseManager.cpp \
    Service/BillService.cpp \
    Dialog/EditBillDialog.cpp \
    Analysis/AnalysisDialog.cpp \
    Service/AiService.cpp \
    Importer/WechatImporter.cpp \
    Importer/AlipayImporter.cpp

# 头文件（带路径）
HEADERS += \
        mainwindow.h \
    Database/BillInfo.h \
    Database/DatabaseManager.h \
    Service/BillService.h \
    Dialog/EditBillDialog.h \
    Analysis/AnalysisDialog.h \
    Service/AiService.h \
    Importer/WechatImporter.h \
    Importer/AlipayImporter.h

# 告诉编译器头文件在哪里找（关键！）
INCLUDEPATH += $$PWD/Headers

# UI文件列表
FORMS += \
    mainwindow.ui \
    Dialog/editbilldialog.ui \
    Analysis/AnalysisDialog.ui

# 部署规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.ini
