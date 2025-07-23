#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "alias.h"
#include<unordered_map>
#include<cassert>

struct CONSTANTS
{
    // Actions
    static chart ACT_CREATE_ROOM ;
    static chart ACT_CREATE_USER ;
    static chart ACT_DELETE_ROOM;
    static chart ACT_DELETE_USER;
    static chart ACT_DISCONNECT;
    static chart ACT_EARLIER_MESS;
    static chart ACT_GET_USERS ;
    static chart ACT_LOGIN ;
    static chart ACT_ROOM_LIST ;
    static chart ACT_SEND_PRIVATE_MESSAGE;
    static chart ACT_SEND_PUBLIC_MESSAGE ;
    static chart ACT_USER_MESSAGE ;

    // Map Parameters
    static chart LF_ACTION;
    static chart LF_INITIATOR ;
    static chart LF_LAST_MSG ;
    static chart LF_LOGIN ;
    static chart LF_PRIVATE_MESSAGE;
    static chart LF_PUBLIC_MESSAGE;
    static chart LF_NAME ;
    static chart LF_PASSWORD;
    static chart LF_REASON ;
    static chart LF_RESULT;
    static chart LF_ROOMLIST;
    static chart LF_ROOMNAME;
    static chart LF_TOKEN;
    static chart LF_USERS;
    static chart LF_USER_RECIEVER;
    static chart LF_USER_TO_DELETE;


    static chart RF_ERROR ;
    static chart RF_SUCCESS;

    //
    static chart RF_ERR_INITIATOR_SERVER;
    static chart RF_ERR_INITIATOR_CHATROOM;
    static chart RF_ERR_PERMISSION_DENIDED;

    static chart LF_DIRECTION;
    static chart RF_DIRECTION_SERVER;
    static chart RF_DIRECTION_CHATROOM;

    //конфиги загрузочные
   static chart CHATROOMS ;
   static chart DB_PASSORD ;
   static chart DB_TABLE_NAME ;
   static chart DB_USER ;
   static chart IP;
   static chart MAX_CONNECTIONS ;
   static chart PORT;
   static chart UNKNOWN;

    ///@brief Длина токена
    static const size_t N_TOKEN_LEN;
    ///@brief
    static const size_t N_MAX_MESSAGE_LEN;
    static const char SERIAL_SYM;

private:
    CONSTANTS() {};
};

enum class ACTIONS {

    LOGIN, DISCONNECT,
    CREATE_USER, DELETE_USER,
    PRIVATE_MESSAGE, PUBLIC_MESSAGE,
    GET_ROOM_USERS, GET_ROOMS_LIST,
    CREATE_ROOM, DELETE_ROOM,
    EARLIER_MESSAGES, SYSTEM
};

extern const std::unordered_map<chart, ACTIONS> _NAME_ACTION;
extern const std::unordered_map<ACTIONS, chart> _ACTION_NAME;
#endif // CONSTANTS_H
