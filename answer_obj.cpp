#include "answer_obj.h"

namespace  ans_obj {

    json_obj MakeErrorObjectTemplate(str_type reason){
    json_obj obj;
    obj.insert(CONSTANTS::LF_RESULT, CONSTANTS::RF_ERROR);
    obj.insert(CONSTANTS::LF_REASON, std::move(reason));
    return obj;
    }

}//namespace


namespace  ans_obj {
//ERROR
json_obj MakeErrorObject(QString reason, ACTIONS act){
    json_obj obj = MakeErrorObjectTemplate(std::move(reason));
    obj.insert(CONSTANTS::LF_INITIATOR, _ACTION_NAME.at(act));
    return obj;
}

json_obj MakeSuccessTemplate(ACTIONS act){
    json_obj obj;
    obj.insert(CONSTANTS::LF_RESULT, CONSTANTS::RF_SUCCESS);
    obj.insert(CONSTANTS::LF_INITIATOR, _ACTION_NAME.at(act));
    return obj;
}

json_obj SuccessCreateUser(QString name){
    json_obj obj = MakeSuccessTemplate(ACTIONS::CREATE_USER);
    obj.insert(CONSTANTS::LF_NAME, std::move(name));
    return obj;
}

json_obj SuccessDisconnect(){
    return MakeSuccessTemplate(ACTIONS::DISCONNECT);
}

json_obj SuccessRoomUsers (str_type roomname, str_type users_serialized){
    json_obj obj = MakeSuccessTemplate(ACTIONS::GET_ROOM_USERS);
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(roomname));
    obj.insert(CONSTANTS::LF_USERS, std::move(users_serialized));
    return obj;
}
json_obj SuccessServerRooms (str_type rooms_serialized){
     json_obj obj = MakeSuccessTemplate(ACTIONS::GET_ROOMS_LIST);
     obj.insert(CONSTANTS::LF_ROOMLIST, std::move(rooms_serialized));
     return obj;
}
json_obj SuccessJoinRoom(str_type token, str_type messages_serialized) {
    json_obj obj = MakeSuccessTemplate(ACTIONS::JOIN_ROOM);
    obj.insert(CONSTANTS::LF_LAST_MSG, std::move(messages_serialized));
    obj.insert(CONSTANTS::LF_TOKEN, std::move(token));
    return obj;
}
json_obj SuccessPrivateMessage(){
    return MakeSuccessTemplate(ACTIONS::PRIVATE_MESSAGE);
}
json_obj SuccessPublicMessage(){
  return MakeSuccessTemplate(ACTIONS::PUBLIC_MESSAGE);
}

json_obj SuccessLogin() {
   return MakeSuccessTemplate(ACTIONS::LOGIN);
}

json_obj TestObject(){
    json_obj obj = MakeSuccessTemplate(ACTIONS::SYSTEM);
    obj.insert(CONSTANTS::LF_TOKEN, "TEST-TEST-TEST");
    return obj;
}

json_obj IncomePublicMessage(const str_type author,str_type msg ){
     json_obj obj = MakeSuccessTemplate(ACTIONS::INCOME_PUBLIC );
     obj.insert(CONSTANTS::LF_NAME, std::move(author));
     obj.insert(CONSTANTS::LF_PUBLIC_MESSAGE, std::move(msg));
     return obj;
}
json_obj IncomePrivateMessage(const str_type author,str_type msg){
      json_obj obj = MakeSuccessTemplate(ACTIONS::INCOME_PRIVATE);
    obj.insert(CONSTANTS::LF_NAME, std::move(author));
    obj.insert(CONSTANTS::LF_PRIVATE_MESSAGE, std::move(msg));
    return obj;
}

}//namespace


namespace ans_obj {

json_obj MakeAdminErrorObject(str_type reason, ADMIN_ACTIONS act){
    json_obj obj = MakeErrorObjectTemplate(std::move(reason));
    obj.insert(CONSTANTS::LF_INITIATOR, _ACTION_ADMIN_NAME.at(act));
    return obj;
}

json_obj MakeSuccessTemplate(ADMIN_ACTIONS act){
    json_obj obj;
    obj.insert(CONSTANTS::LF_RESULT, CONSTANTS::RF_SUCCESS);
    obj.insert(CONSTANTS::LF_INITIATOR, _ACTION_ADMIN_NAME.at(act));
    return obj;
}

json_obj AdminSuccessDeleteRoom(str_type name){
    json_obj obj = MakeSuccessTemplate(ADMIN_ACTIONS::DELETE_ROOM);
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(name));
    return obj;
}

json_obj AdminSuccessDeleteUser(str_type name){
    json_obj obj = MakeSuccessTemplate(ADMIN_ACTIONS::DELETE_USER);
    obj.insert(CONSTANTS::LF_NAME, std::move(name));
    return obj;
}

json_obj AdminSuccessCreateRoom(str_type name){
    json_obj obj = MakeSuccessTemplate(ADMIN_ACTIONS::CREATE_ROOM);
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(name));
    return obj;
}

/////////////////

json_obj AdminSuccessBanUser(){
    json_obj obj = MakeSuccessTemplate(ADMIN_ACTIONS::BAN_USER);
    return obj;
}
json_obj AdminSuccessUnbanUser(){
    json_obj obj = MakeSuccessTemplate(ADMIN_ACTIONS::UNBAN_USER);
    return obj;
}
json_obj AdminSuccessUpdateRoleUser(){
    json_obj obj = MakeSuccessTemplate(ADMIN_ACTIONS::UPDATE_ROLE);
    return obj;
}
json_obj AdminUserList(str_type usetlist){
    json_obj obj = MakeSuccessTemplate(ADMIN_ACTIONS::USER_LIST);
    return obj;
}
json_obj AdminRoomList(str_type roomlist){
    json_obj obj = MakeSuccessTemplate(ADMIN_ACTIONS::ROOM_LIST);
    return obj;
}


}//namespace
