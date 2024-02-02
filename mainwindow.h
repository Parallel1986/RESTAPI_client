#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QSet>
#include <QStandardItemModel>
#include <QStandardItem>

#include "Client.h"


struct CountryInfo
{
    QString capital;
    unsigned long population;
    double area;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void MakeClientWithUrl(QString url);

public slots:
    void RefreshTreeWdget();
    void ShowReply(QJsonDocument reply_json);

private:
    void ParceReplyToMap(QJsonDocument& reply_json);
    void MakeTreeModel();
    QMap<QString/*region*/,QMap<QString/*subregion*/,QMap<QString/*country*/, CountryInfo>>> countries_map;
    Ui::MainWindow *ui = nullptr;
    Client* client = nullptr;
    QNetworkAccessManager* manager = nullptr;
    QStandardItemModel* tree_view_model = nullptr;
};
#endif // MAINWINDOW_H
