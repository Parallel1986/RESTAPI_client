#include "client.h"
#include <QNetworkRequest>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>

Client::Client(QNetworkAccessManager* manager, const QUrl& url):
    manager(manager)
{
    this->url = url;
}

Client::~Client(){}

bool Client::SendRequest(){
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Client","Conutrues_info_application");

    reply = manager->get(request);
    connect(reply, &QNetworkReply::finished,this, &Client::ParceReply);
    connect(reply, &QNetworkReply::errorOccurred, this, &Client::ErrorReply);
    connect(reply, &QNetworkReply::sslErrors, this, &Client::SslErrorReply);
    return true;
}

void Client::ParceReply(){
    if (reply){
        QByteArray rep_info = reply->readAll();
        reply->deleteLater();
        QJsonDocument reply_json;
        reply_json = QJsonDocument::fromJson(rep_info);
        emit RequestResult(reply_json);
    }
}

void Client::ErrorReply(){
    QMessageBox* error_message= new QMessageBox();
    error_message->setText("Error while retrieving reply!");
    error_message->setInformativeText("An error has ocured while trying to receive reply");
    error_message->setStandardButtons(QMessageBox::Ok);
    error_message->setDefaultButton(QMessageBox::Ok);
    error_message->exec();
    delete error_message;
}

void Client::SslErrorReply(){
QMessageBox* error_message= new QMessageBox();
    error_message->setText("Error while retrieving reply!");
    error_message->setInformativeText("An SSL error has ocured while trying to receive reply");
    error_message->setStandardButtons(QMessageBox::Ok);
    error_message->setDefaultButton(QMessageBox::Ok);
    error_message->exec();
    delete error_message;
}
