#ifndef CLIENT_H
#define CLIENT_H

#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QNetworkReply>

class Client:public QObject{
    Q_OBJECT

public:
    Client(QNetworkAccessManager* manager, const QUrl& url);
    ~Client();

    bool SendRequest();

public slots:
    void ParceReply();
    void ErrorReply();
    void SslErrorReply();

signals:
    void RequestResult(QJsonDocument reply);

private:
    QNetworkAccessManager* manager = nullptr;
    QUrl url;
    QNetworkReply* reply = nullptr;
};

#endif // CLIENT_H
