#include "network.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

void HttpClient::get(QString const& url, std::function<void(QJsonDocument)> callback) {
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QJsonDocument document = QJsonDocument::fromJson(data);
            callback(document);
        } else {
            emit error(reply->errorString());
        }
        reply->deleteLater();
    });
}

void HttpClient::get(QString const& url, std::function<void(QByteArray)> callback) {
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            callback(data);
        } else {
            emit error(reply->errorString());
        }
        reply->deleteLater();
    });
}
