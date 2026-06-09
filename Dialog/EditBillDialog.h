#ifndef EDITBILLDIALOG_H
#define EDITBILLDIALOG_H


#include "Database/BillInfo.h"
#include "ui_editbilldialog.h"
#include <QDateTime>
#include <QDialog>
#include <QMessageBox>
//namespace Ui {
//class Dialog;
//}

class EditBillDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditBillDialog(QWidget *parent = nullptr);
    ~EditBillDialog();
public:
    void setBill(const BillInfo& bill);

    BillInfo bill() const;
    void  InitDialogUI();
private slots:
//    void onAccepted();
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    BillInfo m_bill;
};

#endif // EDITBILLDIALOG_H

