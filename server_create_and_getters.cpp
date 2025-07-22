#include "mainwindow.h"

QHostAddress ServerBase::GetIP() const{
    LG(_mtx_net);
    return _ip;
}
QString ServerBase::GetIPStr() const{
    LG(_mtx_net);
    return _ip.toString();
}

int ServerBase::GetPort() const{
    LG(_mtx_net);
    return _port;
}

int ServerBase::GetMaxUsers() const {
    LG(_mtx_net);
    return _max_conn;
}

const std::unordered_set<QString>& ServerBase::GetRooms() const {
    LG(_mtx_room);
    return _roomlist;
}

///////////////////////////////////////////////////////////////////////


void GraphicsServer::InitGraphicForms(){
    _maiwin = std::make_shared<MainWindow>(shared_from_this());
    _maiwindes = std::make_shared<MainWindowDesigner>(_maiwin);
    _rooms_form = std::make_shared<RoomsForm>(shared_from_this());
}

void GraphicsServer::SetDefaultValues(){

    QSpinBox *sb_maxconn = _maiwin->ui->sb_maxconn;
    sb_maxconn->setValue(_max_conn);

    QSpinBox *sb_port = _maiwin->ui->sb_port;
    sb_port->setValue(_port);

    QLineEdit *le_ip = _maiwin->ui->le_setip;
    le_ip->setText(_ip.toString());

    QListWidget *lw_rooms = this->_rooms_form->ui->lw_rooms;
    for(auto && rm : _roomlist){
        lw_rooms->addItem(rm);
        _rooms[rm] = std::make_shared<ChatRoom>(this, "");
    }
}

void GraphicsServer::InitAndRun(){
    InitGraphicForms();
    SetDefaultValues();
    _maiwin->show();
}
