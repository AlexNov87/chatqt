#include "mainwindow.h"
#include "ui_configfile.h"

ConfigFile::ConfigFile(std::shared_ptr<GraphicsServer> srv) :
    _srv(srv) , ui(new Ui::ConfigFile) {
    ui->setupUi(this);
}

ConfigFile::~ConfigFile()
{
    delete ui;
}

void ConfigFile::on_pb_save_to_file_clicked(bool checked)
{
    QJsonObject mainObject;
    // if(ui->sb_save_ip->isChecked()){
    //     mainObject["IP"] =  _inits->GetListenIP().toString();
    // }
    // if(ui->sb_save_port->isChecked()){
    //     mainObject["PORT"] = _inits->GetPort();
    // }
    // if(ui->sb_save_max_conn->isChecked()){
    //     mainObject["MAX_CONNECTIONS"] = _inits->GetMaxConnections();
    // }
    // if(ui->sb_rooms->isChecked()){
    //     QJsonArray rooms;
    //     for(auto && room : _inits->GetRooms()){
    //         rooms.push_back(room);
    //     }
    //     mainObject["ROOMS"] = std::move(rooms);
    // }
    WritetoFileJson(mainObject);
}

