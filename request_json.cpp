#include "request_json.h"
namespace req_obj {

/////////////SERVER
json_obj MakeRequestTemplate(str_type initiator, ACTIONS act){

    if(!IsCorrertInitiator(initiator)){
        FatalErrorMessageBox("Initiator is incorrect");
        std::abort();
    }
    json_obj obj;
    obj.insert(CONSTANTS::LF_DIRECTION, std::move(initiator));
    obj.insert(CONSTANTS::LF_ACTION, _ACTION_NAME.at(act));
    return obj;
}

void ModifyUserAndPassword(json_obj&obj,
                           str_type& name, str_type& password){
    obj.insert(CONSTANTS::LF_NAME, std::move(name));
    obj.insert(CONSTANTS::LF_PASSWORD, std::move(password));
}

//ROOMS
void ModifyRoomReq(json_obj&obj,
                   str_type& name, str_type& password, str_type& roomname){
    ModifyUserAndPassword(obj, name, password);
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(roomname));
}

//USERS
json_obj MakeRequestJoinRoom
    (str_type name, str_type password, str_type roomname){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::JOIN_ROOM);
    //Используется этот метод т.к поля схожи с MakeRequestAddRooms
    ModifyRoomReq(obj, name,password,roomname);
    return obj;
}

json_obj MakeRequestRegisterUser(str_type name, str_type password){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::CREATE_USER);
    ModifyUserAndPassword(obj,name,password);
    return obj;
}

json_obj MakeRequestGetRoomUsers(str_type roomname){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::GET_ROOM_USERS);
    obj.insert(CONSTANTS::LF_ROOMNAME , std::move(roomname));
    return obj;
}

//ROOM
json_obj MakeRequestDisconnect(str_type token, str_type room_name){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_CHATROOM, ACTIONS::DISCONNECT);
    obj.insert(CONSTANTS::LF_TOKEN , std::move(token));
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(room_name));
    return obj;
}

json_obj MakeRequestPublicMessage(str_type token, str_type message, str_type room_name) {
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_CHATROOM, ACTIONS::PUBLIC_MESSAGE);
    obj.insert(CONSTANTS::LF_TOKEN , std::move(token));
    obj.insert(CONSTANTS::LF_PUBLIC_MESSAGE , std::move(message));
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(room_name));
    return obj;
}

json_obj MakeRequestPrivateMessage
    (str_type token, str_type message, str_type user_to, str_type room_name) {
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_CHATROOM, ACTIONS::PRIVATE_MESSAGE);
    obj.insert(CONSTANTS::LF_TOKEN , std::move(token));
    obj.insert(CONSTANTS::LF_PRIVATE_MESSAGE , std::move(message));
    obj.insert(CONSTANTS::LF_USER_RECIEVER , std::move(user_to));
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(room_name));
    return obj;
}

json_obj MakeRequestRoomList(){
 return  MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::GET_ROOMS_LIST);
}
}//namespace

namespace req_obj {

json_obj MakeRequestTemplate(str_type initiator, ADMIN_ACTIONS act){
    if(!IsCorrertInitiator(initiator)){
        FatalErrorMessageBox("Initiator is incorrect");
        std::abort();
    }
    json_obj obj;
    obj.insert(CONSTANTS::LF_DIRECTION, std::move(initiator));
    obj.insert(CONSTANTS::LF_ACTION, _ACTION_ADMIN_NAME.at(act));
    return obj;
}
json_obj AdminMakeRequestAddRooms
    (str_type name, str_type password, str_type roomname)
{
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ADMIN_ACTIONS::CREATE_ROOM);
    ModifyRoomReq(obj, name,password,roomname);
    return obj;
}

json_obj AdminMakeRequestDeleteRooms
    (str_type name, str_type password, str_type roomname)
{
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ADMIN_ACTIONS::DELETE_ROOM);
    ModifyRoomReq(obj, name,password,roomname);
    return obj;
}

json_obj AdminMakeRequestDeleteUser
    (str_type name, str_type password, str_type to_delete){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ADMIN_ACTIONS::DELETE_USER);
    ModifyUserAndPassword(obj, name,password);
    obj.insert(CONSTANTS::LF_USER_RECIEVER, std::move(to_delete));
    return obj;
}

json_obj AdminMakeRequestBanUser
    (str_type name, str_type password, str_type to_ban){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ADMIN_ACTIONS::BAN_USER);
    ModifyUserAndPassword(obj, name,password);
    obj.insert(CONSTANTS::LF_USER_RECIEVER, std::move(to_ban));
    return obj;
}

json_obj AdminMakeRequestUnbanUser
    (str_type name, str_type password, str_type to_unban){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ADMIN_ACTIONS::UNBAN_USER);
    ModifyUserAndPassword(obj, name,password);
    obj.insert(CONSTANTS::LF_USER_RECIEVER, std::move(to_unban));
    return obj;
}

json_obj AdminMakeRequestUpdareRole
    (str_type name, str_type password, str_type to_set_role, Role role){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ADMIN_ACTIONS::UPDATE_ROLE);
    ModifyUserAndPassword(obj, name,password);
    obj.insert(CONSTANTS::LF_USER_RECIEVER, std::move(to_set_role));
    obj.insert(CONSTANTS::LF_ROLE, _ROLE_NAME.at(role));
    return obj;
}

json_obj AdminMakeRequestGetUsers(){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ADMIN_ACTIONS::GET_SERIAL_USERS);
    return obj;
}

}//namespace
