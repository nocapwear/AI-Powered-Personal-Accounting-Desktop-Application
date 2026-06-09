#ifndef AISERVICE_H
#define AISERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QDebug>
#include<QCoreApplication>

class AiService : public QObject
{
    Q_OBJECT

public:
    explicit AiService(QObject *parent = nullptr);

    void requestAnalysis(const QString& prompt);

signals:

    void analysisFinished(const QString& result);

    void analysisFailed(const QString& error);

private:

    QString readApiKey();

private:

    QNetworkAccessManager* m_manager;
};

#endif
