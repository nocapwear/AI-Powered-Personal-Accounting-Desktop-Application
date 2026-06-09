#include "EditBillDialog.h"
#include "ui_editbilldialog.h"

EditBillDialog::EditBillDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::Dialog)
{
    ui->setupUi(this);
    InitDialogUI();

}

EditBillDialog::~EditBillDialog()
{
    delete ui;
}
void  EditBillDialog::InitDialogUI()
{
    ui->cmbType->addItem("支出",0);
    ui->cmbType->addItem("收入",1);

    ui->cmbCategory->addItem("餐饮");
    ui->cmbCategory->addItem("交通");
    ui->cmbCategory->addItem("购物");
    ui->cmbCategory->addItem("娱乐");
    ui->cmbCategory->addItem("住房");
    ui->cmbCategory->addItem("工资");
    ui->cmbCategory->addItem("奖金");
}
void EditBillDialog::setBill(const BillInfo& bill)
{
    m_bill = bill;

    ui->editAmount->setText(
                QString::number(
                    bill.amount));

    ui->cmbType->setCurrentIndex(
                bill.type);

    ui->cmbCategory->setCurrentText(
                bill.category);

    ui->editRemark->setText(
                bill.remark);
}
BillInfo EditBillDialog::bill() const
{
    return m_bill;
}
void EditBillDialog::on_buttonBox_accepted()
{
    if(ui->editAmount->text().isEmpty())
    {
        QMessageBox::warning(this,
                             "提示",
                             "请输入金额");
        return;
    }

    m_bill.amount =
            ui->editAmount
            ->text()
            .toDouble();

    m_bill.type =
            ui->cmbType
            ->currentData()
            .toInt();

    m_bill.category =
            ui->cmbCategory
            ->currentText();

    m_bill.remark =
            ui->editRemark
            ->text();

    m_bill.updateTime =
            QDateTime::currentDateTime()
            .toString(
                "yyyy-MM-dd hh:mm:ss");

    accept();
}
