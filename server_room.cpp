#include"mainwindow.h"
json_obj GraphicsServer::DisconnectJs(str_type token,  str_type room_name) {
    ACTIONS this_act = ACTIONS::DISCONNECT;
    auto lam = [&]{
    LG(_mtx_room);
    if(!_rooms.contains(room_name)){
        return ans_obj::MakeErrorObject("There is no room: " + room_name,
                                        ACTIONS::DISCONNECT);
    }
     return _rooms.at(room_name)->DeleteUserByToken(token);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);

}

json_obj GraphicsServer::PublicMessageJs
    ( str_type token,  str_type message,  str_type room_name) {
    ACTIONS this_act = ACTIONS::PUBLIC_MESSAGE;

    auto lam = [&]{
    LG(_mtx_room);
    if(!_rooms.contains(room_name)){
        return ans_obj::MakeErrorObject("There is no room: " + room_name,
                                        ACTIONS::DISCONNECT);
    }
     return _rooms.at(room_name)->PublicMessage(token,message);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}

json_obj GraphicsServer::PrivateMessageJs
    ( str_type token,  str_type message,
     str_type user_to, str_type room_name) {

    ACTIONS this_act = ACTIONS::PRIVATE_MESSAGE;
    auto lam = [&]{
    LG(_mtx_room);
    if(!_rooms.contains(room_name)){
        return ans_obj::MakeErrorObject("There is no room: " + room_name,
                                        ACTIONS::DISCONNECT);
    }
    return _rooms.at(room_name)->PrivateMessage(token,user_to,message);
    };
    return ans_obj::GuardExceptSetter(lam, this_act);
}
