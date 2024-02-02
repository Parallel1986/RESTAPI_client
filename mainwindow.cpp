#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTreeView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pb_show, &QPushButton::clicked, this, &MainWindow::RefreshTreeWdget);
    manager = new QNetworkAccessManager();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (client != nullptr){
        delete client;
    }
    if (manager != nullptr){
        delete manager;
    }
    if(tree_view_model != nullptr){
        delete tree_view_model;
    }
}

void MainWindow::RefreshTreeWdget(){
    client->SendRequest();
}

void MainWindow::MakeClientWithUrl(QString in_url){
    QUrl url(in_url);
    if (url.isValid())
    {
        client = new Client(manager, url);
        connect (client,&Client::RequestResult, this, &MainWindow::ShowReply);
        ui->l_addres->setText(in_url);
    }
}

void MainWindow::ShowReply(QJsonDocument reply_json){
    ParceReplyToMap(reply_json);
    MakeTreeModel();
    ui->tv_country_list->setModel(tree_view_model);
    ui->tv_country_list->show();
}

void MainWindow::ParceReplyToMap(QJsonDocument& reply_json){
    QJsonArray reply_list = reply_json.array();
    countries_map.clear();
    for (auto country  = reply_list.begin(); country != reply_list.end(); ++country){
        CountryInfo country_info;
        auto cntr = country->toObject();

        QString country_name = cntr.value("name").toObject().value("common").toString();
        country_info.capital = cntr.value("capital").toArray().at(0).toString();
        country_info.area = cntr.value("area").toDouble();
        country_info.population = cntr.value("population").toInt();

        QString country_region = cntr.value("region").toString();
        QString country_subregion = cntr.value("subregion").toString();

        if (countries_map.find(country_region) != countries_map.end()){
            if(countries_map[country_region].find(country_subregion) != countries_map[country_region].end()){
                QMap<QString, CountryInfo> c_inf;
                c_inf.insert(country_name, country_info);
                countries_map[country_region][country_subregion].insert(c_inf);
            }
            else{
                QMap<QString, CountryInfo> c_inf;
                c_inf.insert(country_name, country_info);
                QMap<QString,QMap<QString,CountryInfo>> sr_inf;
                sr_inf.insert(country_subregion,c_inf);

                countries_map[country_region].insert(sr_inf);
            }
        }
        else{
            QMap<QString, CountryInfo> c_inf;
            c_inf.insert(country_name, country_info);

            QMap<QString,QMap<QString,CountryInfo>> sr_inf;
            sr_inf.insert(country_subregion,c_inf);

            countries_map.insert(country_region, sr_inf);
        }
    }    
}

void MainWindow::MakeTreeModel(){
    if (tree_view_model != nullptr){
        delete tree_view_model;
    }
    tree_view_model = new QStandardItemModel;
    QStandardItem* root_item = tree_view_model->invisibleRootItem();

    QStandardItem* header_0 = new QStandardItem("Country");
    tree_view_model->setHorizontalHeaderItem(0,header_0);
    QStandardItem* header_1 = new QStandardItem("Capital");
    tree_view_model->setHorizontalHeaderItem(1,header_1);
    QStandardItem* header_2 = new QStandardItem("Population");
    tree_view_model->setHorizontalHeaderItem(2,header_2);
    QStandardItem* header_3 = new QStandardItem("Area");
    tree_view_model->setHorizontalHeaderItem(3,header_3);

    for (auto region = countries_map.begin(); region != countries_map.end(); ++region){
        QStandardItem* tree_region = new QStandardItem("Region of " + region.key());
        for (auto subregion = region->begin(); subregion != region->end(); ++subregion){
            QStandardItem* tree_subregion = new QStandardItem(((subregion.key() == "") ? "No subregion":"Subregion of " + subregion.key()));
                for(auto country = subregion->begin(); country != subregion->end(); ++country){
                    QStandardItem* country_name= new QStandardItem(country.key());
                    QStandardItem* country_capital= new QStandardItem(country->capital);
                    QStandardItem* country_population= new QStandardItem(QString::number(country->population));
                    QStandardItem* country_area= new QStandardItem(QString::number(country->area));
                    tree_subregion->appendRow({country_name,country_capital,country_population,country_area});
                }
            tree_region->appendRow(tree_subregion);
        }
        root_item->appendRow(tree_region);
    }
}
