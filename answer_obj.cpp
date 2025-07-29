#include "answer_obj.h"

namespace  ans_obj {
//ERROR
json_obj MakeErrorObject(QString reason, ACTIONS act){
    json_obj obj;
    obj.insert(CONSTANTS::LF_RESULT, CONSTANTS::RF_ERROR);
    obj.insert(CONSTANTS::LF_INITIATOR, _ACTION_NAME.at(act));
    obj.insert(CONSTANTS::LF_REASON, std::move(reason));
    return obj;
}

json_obj MakeSuccessTemplate(ACTIONS act){
    json_obj obj;
    obj.insert(CONSTANTS::LF_RESULT, CONSTANTS::RF_SUCCESS);
    obj.insert(CONSTANTS::LF_INITIATOR, _ACTION_NAME.at(act));
    return obj;
}

json_obj SuccessCreateRoom(QString name){
    json_obj obj = MakeSuccessTemplate(ACTIONS::CREATE_ROOM);
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(name));
    return obj;
}

json_obj SuccessCreateUser(QString name){
    json_obj obj = MakeSuccessTemplate(ACTIONS::CREATE_USER);
    obj.insert(CONSTANTS::LF_NAME, std::move(name));
    return obj;
}

json_obj SuccessDeleteRoom(QString name){
    json_obj obj = MakeSuccessTemplate(ACTIONS::DELETE_ROOM);
    obj.insert(CONSTANTS::LF_ROOMNAME, std::move(name));
    return obj;
}
json_obj SuccessDeleteUser(QString name){
    json_obj obj = MakeSuccessTemplate(ACTIONS::DELETE_USER);
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
json_obj SuccessLogin(str_type token, str_type messages_serialized) {
    json_obj obj = MakeSuccessTemplate(ACTIONS::LOGIN);
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

json_obj TestObject(){
    json_obj obj = MakeSuccessTemplate(ACTIONS::SYSTEM);
    obj.insert(CONSTANTS::LF_TOKEN, "TEST-TEST-TEST");
    return obj;
}

}//namespace
