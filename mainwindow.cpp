#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("AIBILL");
    m_pAnalysisDialog =new AnalysisDialog(this);
    connect(this,&MainWindow::billChanged,m_pAnalysisDialog,&AnalysisDialog::OnBillChanged);
    SetGlobalStyle();
    InitTableBill();
    InitComboBox();
    UpdateStatistics();
    loadBills();
    UpdatePieChart();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::SetGlobalStyle()
{
    QString bgPath = "C:/Users/Hush/Desktop/xxx/AIBillAgent/Picture/background.png";
    QString styleSheet = QString(R"(
    /* 主窗口背景：保持原有背景图 */
    QMainWindow {
        border-image: url(%1) 0 0 0 0 stretch stretch;
    }

    /*=====1、左上角拐角【序号】：透明背景+黑色加粗居中=====*/
    QTableWidget::corner {
        min-width:42px;
        max-width:42px;
        background: transparent;
    }
    QTableWidget::corner::item{
        color: #000000;
        font-weight: bold;
        font-size:12px;  /* 字体大小适配空间 */
        text-align: center;
        vertical-align: middle;
    }

    /*=====2、左侧行号（1/2/3/4序号）：透明背景+黑色加粗=====*/
    QHeaderView::vertical{
        background: transparent;
    }
    QHeaderView::vertical::section{
        color: #000000;
        font-weight: bold;
        background: transparent;
        border: none;
        padding: 0 5px;  /* 减小内边距，增加显示空间 */
        font-size:12px;
    }

    /*=====3、顶部表头（金额/类型/分类...）：透明背景+黑色加粗+完整显示=====*/
    QHeaderView::horizontal::section{
        background: transparent;
        color: #000000 !important;
        font-weight: bold;
        padding: 4px 8px;  /* 上下内边距减小，左右保留，避免文字贴边 */
        border-right: 1px solid #cccccc;
        font-size:12px;  /* 关键：字体大小适配表头高度 */
        white-space: nowrap;  /* 禁止文字换行，保持单行完整显示 */
    }
    QHeaderView::horizontal{
        background: transparent;
    }

    /*=====表格主体：半透明背景+黑色文字+加粗=====*/
    QTableWidget {
        background: rgba(255,255,255,0.80);
        border: 1px solid #cccccc;
        gridline-color: #dddddd;
        font-size: 12px;
        color: #000000;
        font-weight: bold;
    }
    QTableWidget::item{
        padding: 4px;
        text-align: center;
        background: transparent;
        color: #000000;
        font-weight: bold;
    }
    QTableWidget::item:alternate{
        background: rgba(245,245,245,0.75);
    }
    QTableWidget::item:selected{
        background: #4285e8;
        color: white !important;
        font-weight: bold;
    }

    /*=====按钮样式：统一大小+文字完整显示+蓝色渐变效果=====*/
                                 QPushButton{
                                 width:132px;
                                 height:38px;
                                 background-color: #507fc8;
                                 color: #000000 !important;
                                 font-weight: bold;
                                 border: none;
                                 border-radius: 7px;
                                 padding: 2px 4px;
                                 font-size: 13px;
                                 qproperty-alignment: AlignCenter;
                                 }
                                 QPushButton:hover{
                                 background: #6996e0;
                                 color: #000000 !important;
                                 }
                                 QPushButton:pressed{
                                 background: #3f69aa;
                                 color: #000000 !important;
                                 }
    )").arg(bgPath);

    this->setStyleSheet(styleSheet);
}
void MainWindow::InitTableBill()
{
    ui->tableBill->setColumnCount(6);
    QStringList headers;
    headers << "ID" << "金额" << "类型" << "分类" << "备注" << "时间";

    ui->tableBill->setHorizontalHeaderLabels(headers);
    ui->tableBill->hideColumn(0);
    ui->tableBill->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableBill->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableBill->setAlternatingRowColors(true);

    // ========== 核心修复：开启横向滚动条 ==========
    QHeaderView* hHeader = ui->tableBill->horizontalHeader();
    QHeaderView* vHeader = ui->tableBill->verticalHeader();

    // 1. 所有列 自适应内容宽度（关键！取消最后一列的Stretch拉伸）
    // 拉伸会禁用横向滚动条，全部改为自适应内容
    for (int i = 0; i < 6; ++i) {
        hHeader->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    // 2. 开启横向滚动条（内容超出宽度自动显示）
    ui->tableBill->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // 保留纵向滚动条（原有功能）
    ui->tableBill->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 3. 表头高度优化
    hHeader->setMinimumHeight(40);
    hHeader->setDefaultAlignment(Qt::AlignCenter);

    // 4. 左侧行号列优化
    vHeader->setMinimumWidth(42);
    vHeader->setDefaultSectionSize(32);
    QFont vHeaderFont = vHeader->font();
    vHeaderFont.setBold(true);
    vHeaderFont.setPointSize(12);
    vHeader->setFont(vHeaderFont);

    // 5. 表头字体
    QFont hHeaderFont = hHeader->font();
    hHeaderFont.setBold(true);
    hHeaderFont.setPointSize(12);
    hHeader->setFont(hHeaderFont);

    // 左上角拐角
    QTableWidgetItem *cornerItem = new QTableWidgetItem("序号");
    cornerItem->setTextAlignment(Qt::AlignCenter);
    cornerItem->setFlags(cornerItem->flags() & ~Qt::ItemIsEditable);
    ui->tableBill->setCornerButtonEnabled(true);
    ui->tableBill->setHorizontalHeaderItem(-1, cornerItem);

    // 表格样式
    ui->tableBill->setFrameShape(QFrame::NoFrame);
    ui->tableBill->setShowGrid(true);
    ui->tableBill->setGridStyle(Qt::DotLine);
}
void MainWindow::loadBills()
{
    QSqlQuery query(DatabaseManager::instance().database());

//    QString sql = "SELECT amount, type, category, remark, create_time FROM bill ORDER BY id DESC";
    QString sql =
    "SELECT id,"
    " amount,"
    " type,"
    " category,"
    " remark,"
    " create_time "
    "FROM bill "
    "ORDER BY id DESC";

    if(!query.exec(sql))
    {
        qDebug() << "query failed:" << query.lastError().text();
        return;
    }

    ui->tableBill->setRowCount(0);

    int row = 0;

    while(query.next())
    {
        ui->tableBill->insertRow(row);

        ui->tableBill->setItem(
            row,
            0,
            new QTableWidgetItem(
                query.value(0).toString()));

        ui->tableBill->setItem(
            row,
            1,
            new QTableWidgetItem(
                query.value(1).toString()));

        QString typeStr =
            query.value(2).toInt() == 0
            ? "支出"
            : "收入";

        ui->tableBill->setItem(
            row,
            2,
            new QTableWidgetItem(typeStr));

        ui->tableBill->setItem(
            row,
            3,
            new QTableWidgetItem(
                query.value(3).toString()));

        ui->tableBill->setItem(
            row,
            4,
            new QTableWidgetItem(
                query.value(4).toString()));

        ui->tableBill->setItem(
            row,
            5,
            new QTableWidgetItem(
                query.value(5).toString()));

        row++;
    }
}

void MainWindow::on_btnAddBill_clicked()
{
    QString amountStr = ui->editAmount->text();

    if(amountStr.isEmpty())
    {
        QMessageBox::warning(this,
                             "提示",
                             "请输入金额");
        return;
    }

    BillInfo bill;

    bill.amount = amountStr.toDouble();

    if(ui->cmbType->currentIndex()==0)
    {
        QMessageBox::warning(this,
                             "提示",
                             "请选择类型");
        return;
    }
    bill.type =
        ui->cmbType->currentData().toInt();

    if(ui->cmbCategory->currentIndex()==0)
    {
        QMessageBox::warning(this,
                             "提示",
                             "请选择分类");
        return;
    }
    bill.category =
        ui->cmbCategory->currentText();

    bill.remark =
        ui->editRemark->text();

    bill.createTime =
        QDateTime::currentDateTime()
            .toString("yyyy-MM-dd hh:mm:ss");

    bill.updateTime =
        bill.createTime;

    BillService service;

    if(service.addBill(bill))
    {
        QMessageBox::information(this,
                                 "提示",
                                 "添加成功");

        loadBills();

        UpdateStatistics();
        UpdatePieChart();

        ClearInput();

        emit billChanged();
    }
}

void MainWindow::InitComboBox()
{
    ui->cmbType->clear();

    ui->cmbType->addItem("请选择",-1);

    ui->cmbType->addItem("支出",0);

    ui->cmbType->addItem("收入",1);


    ui->cmbCategory->clear();

    ui->cmbCategory->addItem("请选择");

    ui->cmbCategory->addItem("餐饮");

    ui->cmbCategory->addItem("交通");

    ui->cmbCategory->addItem("购物");

    ui->cmbCategory->addItem("娱乐");

    ui->cmbCategory->addItem("住房");

    ui->cmbCategory->addItem("工资");

    ui->cmbCategory->addItem("奖金");
}
void MainWindow::ClearInput()
{
    ui->editAmount->clear();

    ui->editRemark->clear();

    ui->cmbType->setCurrentIndex(0);

    ui->cmbCategory->setCurrentIndex(0);
}


void MainWindow::on_btnRefresh_clicked()
{
    loadBills();
    UpdateStatistics();
    UpdatePieChart();
}

void MainWindow::on_btnDeleteBill_clicked()
{
    // 当前选中行
    int row =
            ui->tableBill->currentRow();

    if(row < 0)
    {
        QMessageBox::warning(
                    this,
                    "提示",
                    "请先选择一条账单");

        return;
    }

    // 获取隐藏ID
    int id =
            ui->tableBill
            ->item(row,0)
            ->text()
            .toInt();

    // 二次确认
    auto ret =
            QMessageBox::question(
                this,
                "确认删除",
                "确定删除这条账单吗？",
                QMessageBox::Yes |
                QMessageBox::No);

    if(ret != QMessageBox::Yes)
    {
        return;
    }

    BillService service;

    if(service.deleteBill(id))
    {
        QMessageBox::information(
                    this,
                    "提示",
                    "删除成功");

        loadBills();

        UpdateStatistics();
        UpdatePieChart();
        emit billChanged();
    }
    else
    {
        QMessageBox::warning(
                    this,
                    "提示",
                    "删除失败");
    }
}


void MainWindow::UpdateStatistics()
{
    QSqlQuery query(DatabaseManager::instance().database());
    QString sql = "SELECT type, amount FROM bill";

    if(!query.exec(sql))
    {
        qDebug() << "statistics failed:" << query.lastError().text();
        return;
    }

    double expense = 0;
    double income = 0;
    int count = 0;

    while(query.next())
    {
        int type = query.value(0).toInt();
        double amount = query.value(1).toDouble();

        if(type == 0) expense += amount;
        else income += amount;
        count++;
    }

    double balance = income - expense;

    // ==========关键修复1：启用自动换行+居中对齐（必加）==========
    QList<QLabel*> labels = {
        ui->lblExpense,
        ui->lblIncome,
        ui->lblBalance,
        ui->lblCount
    };

    for (QLabel* label : labels) {
        label->setWordWrap(true);  // 启用自动换行，识别\n
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);  // 水平+垂直居中
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);  // 垂直可扩展
        label->setMinimumHeight(60);  // 确保两行文字高度（根据字体调整）
    }

    // ==========关键修复2：保持原objectname，文本用\n换行（表头在上，数字在下）==========
    ui->lblExpense->setText(QString("总支出：\n¥%1").arg(expense, 0, 'f', 2));
    ui->lblIncome->setText(QString("总收入：\n¥%1").arg(income, 0, 'f', 2));
    ui->lblBalance->setText(QString("当前余额：\n¥%1").arg(balance, 0, 'f', 2));
    ui->lblCount->setText(QString("账单数量：\n%1").arg(count));
}


void MainWindow::on_tableBill_cellDoubleClicked(int row,int column)
{
    BillInfo bill;

    bill.id =
            ui->tableBill
            ->item(row,0)
            ->text()
            .toInt();

    bill.amount =
            ui->tableBill
            ->item(row,1)
            ->text()
            .toDouble();

    bill.type =
            ui->tableBill
            ->item(row,2)
            ->text()=="支出"
            ?0
            :1;

    bill.category =
            ui->tableBill
            ->item(row,3)
            ->text();

    bill.remark =
            ui->tableBill
            ->item(row,4)
            ->text();

    EditBillDialog dlg(this);

    dlg.setBill(bill);

    if(dlg.exec()==QDialog::Accepted)
    {
        BillService service;

        if(service.updateBill(dlg.bill()))
        {
            QMessageBox::information(
                        this,
                        "提示",
                        "修改成功");

            loadBills();

            UpdateStatistics();
            UpdatePieChart();
            emit billChanged();
        }
    }
}

void MainWindow::on_btnSearch_clicked()
{
    QString amount =ui->editAmount->text();

    int type =
            ui->cmbType
            ->currentData()
            .toInt();

    QString category =
            ui->cmbCategory
            ->currentText();

    QString remark =
            ui->editRemark
            ->text();


    BillService service;

    QList<BillInfo> bills =service.searchBills(amount,type,category,remark);
    ui->tableBill->setRowCount(0);
    for(int i=0; i<bills.size();i++)
    {
        const BillInfo& bill =bills[i];

        ui->tableBill->insertRow(i);


        ui->tableBill->setItem(i,0,new QTableWidgetItem(QString::number(bill.id)));
        ui->tableBill->setItem(i,1, new QTableWidgetItem( QString::number(bill.amount)));
        ui->tableBill
                ->setItem(i,
                           2,
                           new QTableWidgetItem(
                               bill.type==0
                               ?"支出"
                               :"收入"));

        ui->tableBill
                ->setItem(i,
                           3,
                           new QTableWidgetItem(
                               bill.category));

        ui->tableBill
                ->setItem(i,
                           4,
                           new QTableWidgetItem(
                               bill.remark));

        ui->tableBill
                ->setItem(i,
                           5,
                           new QTableWidgetItem(
                               bill.createTime));
    }
    UpdateStatistics();
}


void MainWindow::UpdatePieChart()
{
    BillService service;
    QMap<QString, double> data = service.getExpenseStatistics();

    if(data.isEmpty())
    {
        qDebug() << "暂无支出数据";
        return;
    }

    QPieSeries *series = new QPieSeries();
    QList<QColor> colors = {
        QColor("#3498db"), QColor("#e74c3c"), QColor("#2ecc71"),
        QColor("#f39c12"), QColor("#9b59b6")
    };
    int colorIdx = 0;

    double total = 0;
    for(auto val : data) total += val;

    int index = 0; // 用于奇偶交替，防碰撞
    for(auto it = data.begin(); it != data.end(); ++it)
    {
        QString category = it.key();
        double value = it.value();
        double percent = value / total * 100;

        QPieSlice *slice = series->append(category, value);
        slice->setColor(colors[colorIdx++ % colors.size()]);

        // 格式化纯数字百分比字符串
        QString percentStr = QString("%1%").arg(percent, 0, 'f', 1);

        // 饼图上的标签只显示纯百分比
        slice->setLabel(percentStr);
        slice->setLabelVisible(true);

        // 策略分流：根据占比大小调整显示位置与防撞机制
        if (percent > 10)
        {
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
            slice->setLabelFont(QFont("微软雅黑", 9, QFont::Bold));
            slice->setLabelColor(Qt::white);
        }
        else if (percent >= 3)
        {
            slice->setLabelPosition(QPieSlice::LabelOutside);
            slice->setLabelFont(QFont("微软雅黑", 9));
            slice->setLabelColor(Qt::black);

            // 【防撞机制】奇偶交替改变引线长度
            if (index % 2 == 0) {
                slice->setLabelArmLengthFactor(0.12); // 偶数短引线
            } else {
                slice->setLabelArmLengthFactor(0.22); // 奇数长引线
            }
        }
        else
        {
            slice->setLabelVisible(false); // 极小块隐藏
        }

        // 【修复 setToolTip 报错】利用 QtCharts 标准的 hovered 信号实现鼠标悬停提示
        // 构造提示文字
        QString tipText = QString("%1: %2元 (%3)").arg(category).arg(value).arg(percentStr);
        connect(slice, &QPieSlice::hovered, this, [tipText](bool isHovered) {
            if (isHovered) {
                // 鼠标移入时，在当前鼠标位置显示 ToolTip
                QToolTip::showText(QCursor::pos(), tipText);
            } else {
                // 鼠标移出时隐藏
                QToolTip::hideText();
            }
        });

        index++;
    }

    series->setPieSize(0.48);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("消费占比统计");
    chart->setTitleFont(QFont("微软雅黑", 12, QFont::Bold));
    chart->setMargins(QMargins(15, 10, 15, 10));

    // 底部图例基本配置
    QLegend *legend = chart->legend();
    legend->setVisible(true);
    legend->setAlignment(Qt::AlignBottom);
    legend->setFont(QFont("微软雅黑", 9));
    legend->setMarkerShape(QLegend::MarkerShapeCircle);

    // 【修复 invalid use of incomplete type 报错】
    // 只要你在顶部 #include <QLegendMarker>，下面这段强行改图例纯名字的代码就能完美编译通过！
    QList<QLegendMarker *> markers = legend->markers(series);
    auto markerIt = data.begin();
    for (int i = 0; i < markers.size() && markerIt != data.end(); ++i, ++markerIt) {
        markers[i]->setLabel(markerIt.key()); // 强行设为纯名字
    }

    // 渲染配置
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 刷新布局
    QLayout *layout = ui->chartWidget->layout();
    if(!layout)
    {
        layout = new QVBoxLayout(ui->chartWidget);
        layout->setContentsMargins(0, 0, 0, 0);
    }
    while(layout->count())
    {
        QLayoutItem *item = layout->takeAt(0);
        delete item->widget();
        delete item;
    }
    layout->addWidget(chartView);
}

void MainWindow::on_btnAnalysis_clicked()
{
    m_pAnalysisDialog->show();

    m_pAnalysisDialog->raise();

    m_pAnalysisDialog->activateWindow();
}

//void MainWindow::on_actionImportWechat_clicked()
//{
//    qDebug() << "点击了导入微信";
//    QString path =QFileDialog::getOpenFileName(this,"选择微信账单","","CSV Files (*.csv)");

//    if(path.isEmpty())
//    {
//        return;
//    }

//    WechatImporter importer;

//    int count =
//            importer.importFile(path);

//    QMessageBox::information(this,"导入完成",QString("成功导入 %1 条账单").arg(count));

//    //----------------------------------
//    // 自动刷新界面
//    //----------------------------------
//    loadBills();

//    UpdateStatistics();
//}
void MainWindow::on_actionImportWechat_clicked()
{
    QString path =
            QFileDialog::getOpenFileName(
                this,
                "选择微信账单",
                "",
                "CSV Files (*.csv)");

    if(path.isEmpty())
    {
        return;
    }

    WechatImporter importer;

    int count =
            importer.importFile(path);

//    QMessageBox::information(
//                this,
//                "导入完成",
//                QString("成功导入 %1 条账单")
//                .arg(count));

    QMessageBox::information(
                this,
                "导入完成",
                QString("成功导入 %1 条新账单\n重复账单已自动跳过")
                .arg(count));
    loadBills();

    UpdateStatistics();

    UpdatePieChart();
}

void MainWindow::on_actionImportAlipay_clicked()
{

}
