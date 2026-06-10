#ifndef PDFEXPORTER_H
#define PDFEXPORTER_H
#include <QPrinter>
#include <QTextDocument>
#include <QDir>
#include "Service/BillService.h"
#include <QString>
#include <QCoreApplication>
#include <QDateTime>
#include <QTextDocument>
#include <QDesktopServices>
#include <QUrl>
#include <QMarginsF>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QUuid>
#include <QFile>
#include <QtGlobal>
#include <QDebug>
class PdfExporter
{
public:
    static bool exportReport(const QString& aiResult);
//    static QString generateExpenseChart(const QList<double>& values, const QStringList& categories);
};

#endif
