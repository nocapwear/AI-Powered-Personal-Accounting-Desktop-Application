#ifndef ANALYSISDIALOG_H
#define ANALYSISDIALOG_H

#include <QDialog>
#include "Service/BillService.h"
#include "Service/AiService.h"
#include "Reports/PdfExporter.h"
#include "ui_AnalysisDialog.h"
namespace Ui {
class AnalysisDialog;
}

class AnalysisDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AnalysisDialog(QWidget *parent = nullptr);
    ~AnalysisDialog();
private:

    void LoadStatistics();
    QString GeneratePrompt();
private slots:
    void on_btnRefresh_clicked();
    void on_btnAI_clicked();

public slots:
    void OnBillChanged();

private:
    Ui::AnalysisDialog *ui;
    AiService* m_aiService;
};

#endif // ANALYSISDIALOG_H
