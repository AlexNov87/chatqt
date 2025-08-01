#include "mainwindow.h"

QHostAddress ServerBase::GetIP() const{
    LG(_mtx_net);
    return _ip;
}
str_type ServerBase::GetIPStr() const{
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

const std::unordered_map<str_type, std::shared_ptr<ChatRoom>> & ServerBase::GetRooms() const {
    LGR(_mtx_room);
    return _rooms;
}

str_type ServerBase::GetSerializatedRoomList(){

    return json::GetMapMembersJsonArrayView(_rooms);
}

///////////////////////////////////////////////////////////////////////

GraphicsServer::GraphicsServer()  :  ServerBase(), GraphicWidgets() {
    connect(this, &QTcpServer::newConnection,
            this, &GraphicsServer::OnNewConnection);
}

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
    for(auto && rm : _rooms){
        lw_rooms->addItem(rm.first);
    }
}

void GraphicsServer::InitAndRun(){
    InitGraphicForms();
    SetDefaultValues();
    _maiwin->show();
}

QJsonObject GraphicsServer::GetRoomsJs() {

    ACTIONS this_act = ACTIONS::GET_ROOMS_LIST;
    auto lam = [&]{
        str_type roomlist = GetSerializatedRoomList();
        return ans_obj::SuccessServerRooms(roomlist);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

QJsonObject GraphicsServer::GetRoomUsersJs(str_type roomname) {

    ACTIONS this_act = ACTIONS::GET_ROOM_USERS;
    auto lam = [&]{
        LGR(_mtx_room);
        if(!_rooms.contains(roomname)){
            return ans_obj::MakeErrorObject
                ("Not found room "+ roomname , ACTIONS::GET_ROOM_USERS);
        }
        auto room = _rooms.at(roomname);
        auto users = room->SerializatedJsonUsers();
        return ans_obj::SuccessRoomUsers(std::move(roomname), std::move(users));
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::LoginUserJs
    (str_type name,  str_type password) {

    ACTIONS this_act = ACTIONS::LOGIN;
    auto lam = [&]{

        //Если нет добавляющего в базе
        if(_sql_work->IsAuthorizated(name, password)){
            return  ans_obj::MakeErrorObject
                ("You are not authorizated", this_act);
        }

        return ans_obj::SuccessLogin();
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}


