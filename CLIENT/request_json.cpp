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

json_obj MakeRequestAddRooms
    (str_type name, str_type password, str_type roomname)
{
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::CREATE_ROOM);
    ModifyRoomReq(obj, name,password,roomname);
    return obj;
}

json_obj MakeRequestDeleteRooms
    (str_type name, str_type password, str_type roomname)
{
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::DELETE_ROOM);
    ModifyRoomReq(obj, name,password,roomname);
    return obj;
}

//USERS
json_obj MakeRequestLogin
    (str_type name, str_type password, str_type roomname){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::LOGIN);
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

json_obj MakeRequestDeleteUser
    (str_type name, str_type password, str_type to_delete){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::CREATE_USER);
    ModifyUserAndPassword(obj, name,password);
    obj.insert(CONSTANTS::LF_USER_TO_DELETE, std::move(to_delete));
    return obj;
}

json_obj MakeRequestGetRoomUsers(str_type roomname){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_SERVER, ACTIONS::GET_ROOM_USERS);
    obj.insert(CONSTANTS::LF_ROOMNAME , std::move(roomname));
    return obj;
}

//ROOM
json_obj MakeRequestDisconnect(str_type token){
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_CHATROOM, ACTIONS::DISCONNECT);
    obj.insert(CONSTANTS::LF_TOKEN , std::move(token));
    return obj;
}

json_obj MakeRequestPublicMessage(str_type token, str_type message) {
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_CHATROOM, ACTIONS::PUBLIC_MESSAGE);
    obj.insert(CONSTANTS::LF_TOKEN , std::move(token));
    obj.insert(CONSTANTS::LF_PUBLIC_MESSAGE , std::move(token));
    return obj;
}

json_obj MakeRequestPublicMessage
    (str_type token, str_type message, str_type user_to) {
    json_obj obj = MakeRequestTemplate
        (CONSTANTS::RF_DIRECTION_CHATROOM, ACTIONS::PRIVATE_MESSAGE);
    obj.insert(CONSTANTS::LF_TOKEN , std::move(token));
    obj.insert(CONSTANTS::LF_PRIVATE_MESSAGE , std::move(message));
    obj.insert(CONSTANTS::LF_USER_RECIEVER , std::move(user_to));
    return obj;
}


}//namespace
