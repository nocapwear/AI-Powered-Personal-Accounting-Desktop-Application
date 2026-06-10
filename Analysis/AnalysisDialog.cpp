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
        PdfExporter::exportReport(result);
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

//QString AnalysisDialog::GeneratePrompt()
//{
//    BillService service;

//    double income = service.getTotalIncome();
//    double expense = service.getTotalExpense();
//    double balance = income - expense;
//    QStringList topList = service.getTopExpenseCategories();

//    QString prompt;

//    prompt += "【专业财务分析指令】\n";
//    prompt += "角色：国际金融理财师(CFP)，专注个人微观财务诊断，分析客观中立。\n\n";
//    prompt += "【用户财务数据】\n";
//    prompt += QString("总收入：%1 元 | 总支出：%2 元 | 净结余：%3 元\n")
//                 .arg(income, 0, 'f', 2)
//                 .arg(expense, 0, 'f', 2)
//                 .arg(balance, 0, 'f', 2);
//    prompt += "主要支出分类：\n";
//    for (const QString& item : topList) {
//        prompt += QString("• %1\n").arg(item);
//    }

//    prompt += "\n【输出规范】\n";
//    prompt += "1. 专业评价消费结构、支出比率、储蓄水平\n";
//    prompt += "2. 提供精准、可执行的理财建议\n";
//    prompt += "3. 正式报告文风，逻辑清晰，字数≤400字\n";
//    prompt += "4. 禁止虚构数据，严格基于给定信息分析\n";

//    return prompt;
//}
QString AnalysisDialog::GeneratePrompt()
{
    BillService service;

    double income = service.getTotalIncome();
    double expense = service.getTotalExpense();
    double balance = income - expense;

    QStringList topList =
            service.getTopExpenseCategories();

    QString prompt;

    prompt += "你是一名拥有10年以上经验的国际金融理财师（CFP）。";
    prompt += "请基于用户提供的数据，输出专业、客观、简洁的个人财务分析报告。\n\n";

    prompt += "【用户财务数据】\n";

    prompt += QString(
                "总收入：%1 元\n"
                "总支出：%2 元\n"
                "净结余：%3 元\n\n")
            .arg(income,0,'f',2)
            .arg(expense,0,'f',2)
            .arg(balance,0,'f',2);

    prompt += "【消费TOP分类】\n";

    for(const QString& item : topList)
    {
        prompt += item + "\n";
    }

    prompt += "\n";

    prompt += "【输出要求】\n";

    prompt += "请严格按照下面格式输出：\n\n";

    prompt += "【消费结构分析】\n";
    prompt += "内容控制在2~3句话。\n\n";

    prompt += "【储蓄能力评价】\n";
    prompt += "内容控制在2~3句话。\n\n";

    prompt += "【财务风险提示】\n";
    prompt += "内容控制在2~3句话。\n\n";

    prompt += "【优化建议】\n";
    prompt += "1. 建议一\n";
    prompt += "2. 建议二\n";
    prompt += "3. 建议三\n\n";

    prompt += "【重要限制】\n";
    prompt += "1. 禁止使用Markdown格式；\n";
    prompt += "2. 禁止输出###、##、**、---等符号；\n";
    prompt += "3. 禁止输出表格；\n";
    prompt += "4. 必须使用纯文本；\n";
    prompt += "5. 各模块之间必须空一行；\n";
    prompt += "6. 全文控制在400字以内；\n";
    prompt += "7. 严格基于提供的数据分析，禁止虚构数据。";

    return prompt;
}
void AnalysisDialog::on_btnAI_clicked()
{
    QString prompt =GeneratePrompt();

    ui->textAI->setText("正在请求DeepSeek...");

    m_aiService->requestAnalysis(prompt);
}
