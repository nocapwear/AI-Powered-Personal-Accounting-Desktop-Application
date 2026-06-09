#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Service/BillService.h"
#include "Database/DatabaseManager.h"
#include "Dialog/EditBillDialog.h"
#include "Analysis/AnalysisDialog.h"
#include "Importer/WechatImporter.h"
#include "Importer/AlipayImporter.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>   // 必须添加，解决 QSqlError 不完整类型问题
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QLegendMarker>
#include <QToolTip>
#include <QCursor>
#include<QFileDialog>
// 原有的其他头文件保持不变...

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void loadBills();
    void SetGlobalStyle();
    void InitTableBill();
    void InitComboBox();
    void ClearInput();
    void UpdateStatistics();
    void UpdatePieChart();
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void billChanged();

private slots:

    void on_btnAddBill_clicked();

    void on_btnRefresh_clicked();

    void on_btnDeleteBill_clicked();

    void on_tableBill_cellDoubleClicked(int row, int column);

    void on_btnSearch_clicked();

    void on_btnAnalysis_clicked();

    void on_actionImportWechat_clicked();

    void on_actionImportAlipay_clicked();

private:
    Ui::MainWindow *ui;
    AnalysisDialog* m_pAnalysisDialog = nullptr;
};

#endif // MAINWINDOW_H
