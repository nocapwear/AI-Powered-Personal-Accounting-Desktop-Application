#include "AiService.h"
AiService::AiService(QObject *parent)
    : QObject(parent)
{
    m_manager =
            new QNetworkAccessManager(this);
}

QString AiService::readApiKey()
{
    QString path =
        QCoreApplication::applicationDirPath()
        + "/config.ini";

    qDebug() << "Config Path:" << path;

    QSettings setting(path,
                      QSettings::IniFormat);

    QString key =
        setting.value("DeepSeek/ApiKey").toString();

    qDebug() << "ApiKey Length:" << key.length();

    return key;
}

void AiService::requestAnalysis(const QString &prompt)
{
    QString apiKey =readApiKey();

    if(apiKey.isEmpty())
    {
        emit analysisFailed("ApiKey为空");

        return;
    }

    QNetworkRequest request;

    request.setUrl(
                QUrl("https://api.deepseek.com/chat/completions"));

    request.setHeader(
                QNetworkRequest::ContentTypeHeader,
                "application/json");

    request.setRawHeader(
                "Authorization",
                QString("Bearer %1")
                .arg(apiKey)
                .toUtf8());

    //-------------------------------------
    // JSON
    //-------------------------------------

    QJsonObject root;

    root["model"] =
            "deepseek-chat";

    root["stream"] =
            false;

    QJsonArray messages;

    QJsonObject userMsg;

    userMsg["role"] =
            "user";

    userMsg["content"] =
            prompt;

    messages.append(userMsg);

    root["messages"] =
            messages;

    QByteArray data =
            QJsonDocument(root)
            .toJson();

    qDebug()
            << "Request Json:"
            << data;

    //-------------------------------------
    // POST
    //-------------------------------------

    QNetworkReply* reply =
            m_manager->post(
                request,
                data);

    connect(reply,
            &QNetworkReply::finished,
            this,
            [=]()
    {
        if(reply->error()
                != QNetworkReply::NoError)
        {
            emit analysisFailed(
                        reply->errorString());

            reply->deleteLater();

            return;
        }

        QByteArray response =
                reply->readAll();

        qDebug()
                << "Response:"
                << response;

        //---------------------------------
        // Parse JSON
        //---------------------------------

        QJsonDocument doc =
                QJsonDocument::fromJson(
                    response);

        if(!doc.isObject())
        {
            emit analysisFailed(
                        "JSON解析失败");

            reply->deleteLater();

            return;
        }

        QJsonObject obj =
                doc.object();

        QString content;

        QJsonArray choices =
                obj["choices"]
                .toArray();

        if(!choices.isEmpty())
        {
            QJsonObject choice =
                    choices.at(0)
                    .toObject();

            QJsonObject message =
                    choice["message"]
                    .toObject();

            content =
                    message["content"]
                    .toString();
        }

        if(content.isEmpty())
        {
            emit analysisFailed(
                        "AI返回为空");
        }
        else
        {
            emit analysisFinished(
                        content);
        }

        reply->deleteLater();
    });
}
