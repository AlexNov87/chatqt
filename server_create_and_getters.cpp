#include "mainwindow.h"
///////////////////////////////////////////////////////////////////////

GraphicsServer::GraphicsServer()  :  ServerBase(), GraphicWidgets() {
    connect(this, &QTcpServer::newConnection,
            this, &GraphicsServer::OnNewConnection);
}

void GraphicsServer::InitGraphicForms(){
    _maiwin = std::make_shared<MainWindow>(shared_from_this());
    _maiwindes = std::make_shared<MainWindowDesigner>(_maiwin);
}

void GraphicsServer::SetDefaultValues(){

    QSpinBox *sb_maxconn = _maiwin->ui->sb_maxconn;
    sb_maxconn->setValue(_max_conn);

    QSpinBox *sb_port = _maiwin->ui->sb_port;
    sb_port->setValue(_port);

    QLineEdit *le_ip = _maiwin->ui->le_setip;
    le_ip->setText(_ip.toString());
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

        if(auto res =_sql_work->AuthorizatedError
                       (name, password, this_act)){
            return  *res;
        }

        return ans_obj::SuccessLogin();
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}


