#include "AnalysisDialog.h"

AnalysisDialog::AnalysisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalysisDialog)
{
    ui->setupUi(this);
    m_aiService =new AiService(this);
    connect(m_aiService,&AiService::analysisFinished,this,[=](const QString& result)
    {
        ui->textAI->setText(result);
    });
    connect(m_aiService,&AiService::analysisFailed,this,[=](const QString& error)
    {
        ui->textAI->setText(
                    "分析失败："
                    + error);
    });
    LoadStatistics();
}

AnalysisDialog::~AnalysisDialog()
{
    delete ui;
}
void AnalysisDialog::LoadStatistics()
{
    BillService service;

    double income =service.getTotalIncome();

    double expense = service.getTotalExpense();

    double balance =
            income - expense;


    ui->labelIncome->setText(
                QString("总收入：%1 元")
                .arg(income,0,'f',2));


    ui->labelExpense->setText(
                QString("总支出：%1 元")
                .arg(expense,0,'f',2));


    ui->labelBalance->setText(
                QString("净收入：%1 元")
                .arg(balance,0,'f',2));

    ui->listTopExpense->clear();

    QStringList topList =
            service.getTopExpenseCategories();

    ui->listTopExpense->addItems(topList);
}

void AnalysisDialog::on_btnRefresh_clicked()
{
    LoadStatistics();
}
void AnalysisDialog::OnBillChanged()
{
    LoadStatistics();
    ui->textAI->setText(
            "账单已更新，请重新生成AI分析");
    qDebug() << "Analysis Refresh";
}
QString AnalysisDialog::GeneratePrompt()
{
    BillService service;

    double income =service.getTotalIncome();

    double expense =service.getTotalExpense();

    double balance =income - expense;

    QStringList topList =service.getTopExpenseCategories();

    QString prompt;

    prompt +="你是一名专业财务顾问。\n\n";

    prompt +="请根据以下财务数据进行分析：\n\n";

    prompt +=QString("总收入：%1元\n").arg(income,0,'f',2);

    prompt +=
            QString("总支出：%1元\n")
            .arg(expense,0,'f',2);

    prompt +=QString("净收入：%1元\n\n").arg(balance,0,'f',2);

    prompt +="消费TOP3：\n";

    for(const QString& item : topList)
    {
        prompt += item + "\n";
    }
    prompt +=
            "\n请从以下方面分析：\n"
            "1.消费结构是否合理\n"
            "2.支出是否偏高\n"
            "3.储蓄能力评价\n"
            "4.给出具体建议\n"
            "5.控制在200字以内\n";
    return prompt;
}

void AnalysisDialog::on_btnAI_clicked()
{
    QString prompt =GeneratePrompt();

    ui->textAI->setText("正在请求DeepSeek...");

    m_aiService->requestAnalysis(prompt);
}
