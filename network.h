#ifndef NETWORK_H_
#define NETWORK_H_

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QString>
#include <QUrl>
#include <functional>

#define API "https://rickandmortyapi.com/api"

class HttpClient : public QObject {
    Q_OBJECT

  public:
    explicit HttpClient(QObject *parent = nullptr);

    void get(QString const& url, std::function<void(QJsonDocument)> callback);
    void get(QString const& url, std::function<void(QByteArray)> callback);

  signals:
    void error(const QString &message);

  private:
    QNetworkAccessManager *manager;
};

#endif // NETWORK_H_
