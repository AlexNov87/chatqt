#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "alias.h"
#include<map>
#include<set>
#include<QString>

struct CONSTANTS
{
    // Actions
    static chart ACT_CREATE_USER ;
    static chart ACT_DISCONNECT;
    static chart ACT_EARLIER_MESS;
    static chart ACT_GET_USERS;
    static chart ACT_INCOME_PRIVATE;
    static chart ACT_INCOME_PUBLIC;
    static chart ACT_JOIN_ROOM;
    static chart ACT_LOGIN;
    static chart ACT_ROOM_LIST ;
    static chart ACT_SEND_PRIVATE_MESSAGE;
    static chart ACT_SEND_PUBLIC_MESSAGE;
    static chart ACT_SYSTEM;

    //admins
    static chart ADMIN_ACT_BAN_USER;
    static chart ADMIN_ACT_CREATE_ROOM;
    static chart ADMIN_ACT_DELETE_USER;
    static chart ADMIN_ACT_DELETE_ROOM;
    static chart ADMIN_ACT_FIND_USERS;
    static chart ADMIN_ACT_SYSTEM;
    static chart ADMIN_ACT_UNBAN_USER;
    static chart ADMIN_ACT_UPDATE_ROLE;
    static chart ADMIN_ACT_USER_LIST;
    static chart ADMIN_ACT_ROOM_LIST;


    // Map Parameters
    static chart LF_ACTION;
    static chart LF_DIRECTION;
    static chart LF_INITIATOR ;
    static chart LF_LAST_MSG ;
    static chart LF_LOGIN;
    static chart LF_PRIVATE_MESSAGE;
    static chart LF_PUBLIC_MESSAGE;
    static chart LF_NAME ;
    static chart LF_PASSWORD;
    static chart LF_REASON ;
    static chart LF_RESULT;
    static chart LF_ROLE;
    static chart LF_ROOMLIST;
    static chart LF_ROOMNAME;
    static chart LF_TOKEN;
    static chart LF_USERS;
    static chart LF_USER_RECIEVER;
    static chart LF_VALUE;


    static chart RF_ERROR ;
    static chart RF_SUCCESS;
    static chart RF_THREEDOTS;

    static chart RF_ERR_PERMISSION_DENIDED;

    static chart RF_DIRECTION_SERVER;
    static chart RF_DIRECTION_CHATROOM;
    static chart UNKNOWN;

    static const char SERIAL_SYM;

    static chart ROLE_MASTER;
    static chart ROLE_ADMINISTRATOR;
    static chart ROLE_SUPERUSER;
    static chart ROLE_USER;

private:
    CONSTANTS() {}
};

enum class ACTIONS {

    LOGIN, DISCONNECT, CREATE_USER,
    PRIVATE_MESSAGE, PUBLIC_MESSAGE,
    GET_ROOM_USERS, GET_ROOMS_LIST,
    JOIN_ROOM, SYSTEM,
    INCOME_PUBLIC, INCOME_PRIVATE
};

enum class ADMIN_ACTIONS {
    BAN_USER,CREATE_ROOM,
    DELETE_ROOM, DELETE_USER,
    FIND_USERS,  SYSTEM,
    UPDATE_ROLE, UNBAN_USER,
    USER_LIST, ROOM_LIST
};

enum class Role {
    USER , SUPERUSER , ADMIN , MASTER
};

struct UserRole{
    str_type password;
    Role role;
    bool is_active;
};

extern const std::map<str_type, ACTIONS> _NAME_ACTION;
extern const std::map<ACTIONS, str_type> _ACTION_NAME;
extern const std::set<str_type> _ACT_SERVER;
extern const std::set<str_type> _ACT_USER;

extern const std::map<str_type, ADMIN_ACTIONS> _NAME_ADMIN_ACTION;
extern const std::map<ADMIN_ACTIONS, str_type> _ACTION_ADMIN_NAME;
extern const std::set<str_type> _ACT_ADMIN;

extern const std::map<str_type, Role> _NAME_ROLE;
extern const std::map<Role, str_type> _ROLE_NAME;

#endif // CONSTANTS_H
