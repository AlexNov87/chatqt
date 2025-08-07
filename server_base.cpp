#include "structs_ch.h"

ServerBase::ServerBase() : QTcpServer(){
    ConfigInit init;
    _sql_work = std::make_shared<sql::SQLWorker>(init.Object());
    _ip.setAddress(init.Object().value(LOAD_CONSTANTS::SERVER_IP).toString());
    _port = init.Object().value(LOAD_CONSTANTS::SERVER_PORT).toInt();
    _max_conn = init.Object().value(LOAD_CONSTANTS::MAX_CONNECTIONS).toInt();
    _max_message_len = init.Object().value(LOAD_CONSTANTS::MAX_MESSAGE_LEN).toInt();

    for(auto&& el : init.Object().value(LOAD_CONSTANTS::DEFAULT_CHATROOMS).toArray()){
        _rooms[el.toString()] = std::make_shared<ChatRoom>(this, "DEFAULT", el.toString());
    }
}

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

std::optional<json_obj> ServerBase::AuthorizatedAndHasPermissionAdmin
    (QString name, QString password, ADMIN_ACTIONS act){
    if(auto res =_sql_work->AuthorizatedError
                   (name, password, act)){
        return  *res;
    }


    if(!HasPermissionAdmin(name, password, act)){
        return  ans_obj::MakeAdminErrorObject
            ("You have no permission to do this", act);
    }
    return std::nullopt;

}
